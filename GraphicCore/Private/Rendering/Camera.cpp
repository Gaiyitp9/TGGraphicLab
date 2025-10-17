/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Rendering/Camera.h"

#include "Diagnostic/Log.hpp"
#include "Rendering/ViewProjectionMatrix.hpp"
#include "Input/Devices.h"

namespace TG
{
    Camera::Camera(const std::weak_ptr<IDefaultVideoPort>& videoPort, const std::weak_ptr<ITimer>& timer)
        : m_videoPort(videoPort), m_timer(timer)
    {
        UpdateCameraVectors();
    }

    Camera::~Camera() = default;

    Math::Matrix4f Camera::ViewMatrix() const
    {
        return Rendering::LookAt<float>(position, position + front, m_worldUp);
    }

    Math::Matrix4f Camera::ProjectionMatrix() const
    {
        if (m_videoPort.expired())
            return Math::Matrix4f::Identity();

        const auto videoPortPtr = m_videoPort.lock();
        const float aspectRatio = static_cast<float>(videoPortPtr->Width()) /
            static_cast<float>(videoPortPtr->Height());
        switch (cameraType)
        {
            case CameraType::Perspective:
                return Rendering::Perspective(fov * Math::Deg2RadF, aspectRatio, nearPlane, farPlane);

            case CameraType::Orthographic:
            {
                float orthoHeight = orthoWidth / aspectRatio;
                return Rendering::Orthographic(-orthoWidth, orthoWidth, -orthoHeight, orthoHeight,
                    nearPlane, farPlane);
            }

            default:
                return Math::Matrix4f::Identity();
        }
    }

    Frustum Camera::ViewFrustum() const
    {
        if (m_videoPort.expired())
            return {};
        const auto videoPortPtr = m_videoPort.lock();
        const float aspectRatio = static_cast<float>(videoPortPtr->Width()) /
            static_cast<float>(videoPortPtr->Height());
        Math::Vector3f nearFront = front.Normalized() * nearPlane;
        Math::Vector3f nearUp = nearPlane * std::tan(fov * Math::Deg2RadF * 0.5f) * up;
        Math::Vector3f nearRight = nearUp.Norm() * aspectRatio * right;
        Math::Vector3f farFront = front.Normalized() * farPlane;
        Math::Vector3f farUp = farPlane * std::tan(fov * Math::Deg2RadF * 0.5f) * up;
        Math::Vector3f farRight = farUp.Norm() * aspectRatio * right;

        Frustum frustum;
        frustum.corners[0] = position + nearFront - nearRight - nearUp;
        frustum.corners[1] = position + nearFront + nearRight - nearUp;
        frustum.corners[2] = position + nearFront + nearRight + nearUp;
        frustum.corners[3] = position + nearFront - nearRight + nearUp;
        frustum.corners[4] = position + farFront - farRight - farUp;
        frustum.corners[5] = position + farFront + farRight - farUp;
        frustum.corners[6] = position + farFront + farRight + farUp;
        frustum.corners[7] = position + farFront - farRight + farUp;

        return frustum;
    }

    float Camera::AspectRatio() const
    {
        if (m_videoPort.expired())
            return 0.0f;

        const auto videoPortPtr = m_videoPort.lock();
        const float aspectRatio = static_cast<float>(videoPortPtr->Width()) /
            static_cast<float>(videoPortPtr->Height());
        return aspectRatio;
    }

    void Camera::Update()
    {
        if (m_timer.expired())
            return;

        auto timer = m_timer.lock();
        float velocity = m_moveSpeed * timer->DeltaTime() * 0.001f;
        if (Input::GetKey(Input::KeyCode::W))
            position += velocity * front;
        if (Input::GetKey(Input::KeyCode::S))
            position -= velocity * front;
        if (Input::GetKey(Input::KeyCode::A))
            position -= velocity * right;
        if (Input::GetKey(Input::KeyCode::D))
            position += velocity * right;
        if (Input::GetKey(Input::KeyCode::Q))
            position -= velocity * m_worldUp;
        if (Input::GetKey(Input::KeyCode::E))
            position += velocity * m_worldUp;

        if (Input::GetKeyDown(Input::KeyCode::RightMouseButton))
        {
            ShowCursor(false);
            POINT screenPos;
            GetCursorPos(&screenPos);
            m_clickMouseX = static_cast<short>(screenPos.x);
            m_clickMouseY = static_cast<short>(screenPos.y);
            m_lastMouseX = m_clickMouseX;
            m_lastMouseY = m_clickMouseY;
        }

        if (Input::GetKey(Input::KeyCode::RightMouseButton))
        {
            POINT screenPos;
            GetCursorPos(&screenPos);
            const float xOffset = static_cast<float>(screenPos.x - m_lastMouseX) * m_mouseSensitivity;
            const float yOffset = static_cast<float>(screenPos.y - m_lastMouseY) * m_mouseSensitivity;

            POINT clientPos = screenPos;
            ScreenToClient(m_videoPort.lock()->Handle(), &clientPos);
            bool resetPosition = false;
            if (clientPos.x > m_videoPort.lock()->Width())
            {
                clientPos.x = 0;
                resetPosition = true;
            }
            if (clientPos.x < 0)
            {
                clientPos.x = static_cast<short>(m_videoPort.lock()->Width());
                resetPosition = true;
            }
            if (clientPos.y > m_videoPort.lock()->Height())
            {
                clientPos.y = 0;
                resetPosition = true;
            }
            if (clientPos.y < 0)
            {
                clientPos.y = static_cast<short>(m_videoPort.lock()->Height());
                resetPosition = true;
            }
            if (resetPosition)
            {
                screenPos = clientPos;
                ClientToScreen(m_videoPort.lock()->Handle(), &screenPos);
                SetCursorPos(screenPos.x, screenPos.y);
            }

            m_lastMouseX = static_cast<short>(screenPos.x);
            m_lastMouseY = static_cast<short>(screenPos.y);

            m_yaw -= xOffset;
            m_pitch = std::clamp(m_pitch - yOffset, -89.0f, 89.0f);

            UpdateCameraVectors();
        }

        if (Input::GetKeyUp(Input::KeyCode::RightMouseButton))
        {
            ShowCursor(true);
            SetCursorPos(m_clickMouseX, m_clickMouseY);
        }

        fov = std::clamp(fov - static_cast<float>(Input::MouseWheelDelta()), 1.0f, 80.0f);
        orthoWidth = std::clamp(orthoWidth - static_cast<float>(Input::MouseWheelDelta()) * 0.1f, 0.2f, 20.0f);
    }

    void Camera::UpdateCameraVectors()
    {
        front.X() = -std::cos(m_pitch * Math::Deg2RadF) * std::sin(m_yaw * Math::Deg2RadF);
        front.Y() = std::sin(m_pitch * Math::Deg2RadF);
        front.Z() = -std::cos(m_pitch * Math::Deg2RadF) * std::cos(m_yaw * Math::Deg2RadF);
        right = front.Cross(m_worldUp).Normalized();
        up = right.Cross(front);
    }
}
