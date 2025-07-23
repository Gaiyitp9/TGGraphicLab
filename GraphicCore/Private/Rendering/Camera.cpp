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

    Math::Matrix4F Camera::ViewMatrix() const
    {
        return Rendering::LookAt<float>(m_position, m_position + m_front, m_worldUp);
    }

    Math::Matrix4F Camera::ProjectionMatrix() const
    {
        if (m_videoPort.expired())
            return Math::Matrix4F::Identity();

        const auto videoPortPtr = m_videoPort.lock();
        const float aspectRatio = static_cast<float>(videoPortPtr->Width()) /
            static_cast<float>(videoPortPtr->Height());
        switch (m_cameraType)
        {
            case CameraType::Perspective:
                return Rendering::Perspective(m_fov * Math::Deg2RadF, aspectRatio, m_nearPlane, m_farPlane);

            case CameraType::Orthographic:
            {
                float orthoHeight = m_orthoWidth / aspectRatio;
                return Rendering::Orthographic(-m_orthoWidth, m_orthoWidth, -orthoHeight, orthoHeight,
                    m_nearPlane, m_farPlane);
            }

            default:
                return Math::Matrix4F::Identity();
        }
    }

    Frustum Camera::ViewFrustum() const
    {
        if (m_videoPort.expired())
            return {};
        const auto videoPortPtr = m_videoPort.lock();
        const float aspectRatio = static_cast<float>(videoPortPtr->Width()) /
            static_cast<float>(videoPortPtr->Height());
        Math::Vector3F nearFront = m_front.Normalized() * m_nearPlane;
        Math::Vector3F nearUp = m_nearPlane * std::tan(m_fov * Math::Deg2RadF * 0.5f) * m_up;
        Math::Vector3F nearRight = nearUp.Norm() * aspectRatio * m_right;
        Math::Vector3F farFront = m_front.Normalized() * m_farPlane;
        Math::Vector3F farUp = m_farPlane * std::tan(m_fov * Math::Deg2RadF * 0.5f) * m_up;
        Math::Vector3F farRight = farUp.Norm() * aspectRatio * m_right;

        Frustum frustum;
        frustum.corners[0] = m_position + nearFront - nearRight - nearUp;
        frustum.corners[1] = m_position + nearFront + nearRight - nearUp;
        frustum.corners[2] = m_position + nearFront + nearRight + nearUp;
        frustum.corners[3] = m_position + nearFront - nearRight + nearUp;
        frustum.corners[4] = m_position + farFront - farRight - farUp;
        frustum.corners[5] = m_position + farFront - farRight + farUp;
        frustum.corners[6] = m_position + farFront + farRight + farUp;
        frustum.corners[7] = m_position + farFront - farRight + farUp;

        return frustum;
    }

    void Camera::Update()
    {
        if (m_timer.expired())
            return;

        auto timer = m_timer.lock();
        float velocity = m_moveSpeed * timer->DeltaTime() * 0.001f;
        if (Input::GetKey(Input::KeyCode::W))
            m_position += velocity * m_front;
        if (Input::GetKey(Input::KeyCode::S))
            m_position -= velocity * m_front;
        if (Input::GetKey(Input::KeyCode::A))
            m_position -= velocity * m_right;
        if (Input::GetKey(Input::KeyCode::D))
            m_position += velocity * m_right;
        if (Input::GetKey(Input::KeyCode::Q))
            m_position -= velocity * m_worldUp;
        if (Input::GetKey(Input::KeyCode::E))
            m_position += velocity * m_worldUp;

        if (Input::GetKeyDown(Input::KeyCode::RightMouseButton))
        {
            // ShowCursor(false);
            m_clickMouseX = Input::MousePositionX();
            m_clickMouseY = Input::MousePositionY();
            m_lastMouseX = m_clickMouseX;
            m_lastMouseY = m_clickMouseY;
        }

        if (Input::GetKey(Input::KeyCode::RightMouseButton))
        {
            const float xOffset = static_cast<float>(Input::MousePositionX() - m_lastMouseX) * m_mouseSensitivity;
            const float yOffset = static_cast<float>(Input::MousePositionY() - m_lastMouseY) * m_mouseSensitivity;
            if (xOffset > 5.0f || xOffset < -5.0f)
            {
                short mouseX = Input::MousePositionX();
                short mouseY = Input::MousePositionY();
                LogInfo("x offset: {}", xOffset);
            }
            m_lastMouseX = Input::MousePositionX();
            m_lastMouseY = Input::MousePositionY();
            bool resetPosition = false;
            if (m_lastMouseX > m_videoPort.lock()->Width())
            {
                m_lastMouseX = 0;
                resetPosition = true;
            }
            if (m_lastMouseX < 0)
            {
                m_lastMouseX = static_cast<short>(m_videoPort.lock()->Width());
                resetPosition = true;
            }
            if (m_lastMouseY > m_videoPort.lock()->Height())
            {
                m_lastMouseY = 0;
                resetPosition = true;
            }
            if (m_lastMouseY < 0)
            {
                m_lastMouseY = static_cast<short>(m_videoPort.lock()->Height());
                resetPosition = true;
            }
            if (resetPosition)
            {
                POINT screenPos{ m_lastMouseX, m_lastMouseY };
                ClientToScreen(m_videoPort.lock()->Handle(), &screenPos);
                SetCursorPos(screenPos.x, screenPos.y);
                GetCursorPos(&screenPos);
                ScreenToClient(m_videoPort.lock()->Handle(), &screenPos);
                LogInfo("Cursor now at: {}, {}", screenPos.x, screenPos.y);  // 立即打印新坐标
                // LPARAM lParam = MAKELPARAM(m_lastMouseX, m_lastMouseY);
                // WPARAM wParam = 0; // 没有鼠标键按下
                // PostMessage(m_videoPort.lock()->Handle(), WM_MOUSEMOVE, wParam, lParam);
            }

            m_yaw -= xOffset;
            m_pitch = std::clamp(m_pitch - yOffset, -89.0f, 89.0f);

            UpdateCameraVectors();
        }

        if (Input::GetKeyUp(Input::KeyCode::RightMouseButton))
        {
            // ShowCursor(true);
            POINT screenPos{ m_clickMouseX, m_clickMouseY };
            ClientToScreen(m_videoPort.lock()->Handle(), &screenPos);
            SetCursorPos(screenPos.x, screenPos.y);
        }

        m_fov = std::clamp(m_fov - static_cast<float>(Input::MouseWheelDelta()), 1.0f, 80.0f);
        m_orthoWidth = std::clamp(m_orthoWidth - static_cast<float>(Input::MouseWheelDelta()) * 0.1f, 0.2f, 20.0f);
    }

    void Camera::UpdateCameraVectors()
    {
        m_front.X() = -std::cos(m_pitch * Math::Deg2RadF) * std::sin(m_yaw * Math::Deg2RadF);
        m_front.Y() = std::sin(m_pitch * Math::Deg2RadF);
        m_front.Z() = -std::cos(m_pitch * Math::Deg2RadF) * std::cos(m_yaw * Math::Deg2RadF);
        m_right = m_front.Cross(m_worldUp);
        m_up = m_right.Cross(m_front);
    }
}
