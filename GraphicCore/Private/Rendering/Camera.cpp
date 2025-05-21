/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Rendering/Camera.h"
#include "Rendering/ViewProjectionMatrix.hpp"
#include "Input/Devices.h"

namespace TG
{
    Camera::Camera(const std::weak_ptr<IDefaultVideoPort>& videoPort, const std::weak_ptr<ITimer>& timer)
        : m_videoPort(videoPort), m_timer(timer) {}

    Camera::~Camera() = default;

    Math::Matrix4F Camera::ViewMatrix() const
    {
        return Rendering::LookAt<float>(m_position, m_position + m_front, m_worldUp);
    }

    Math::Matrix4F Camera::ProjectionMatrix() const
    {
        switch (m_cameraType)
        {
            case CameraType::Perspective:
            {
                auto videoPortPtr = m_videoPort.lock();
                float aspectRatio = static_cast<float>(videoPortPtr->Width()) /
                    static_cast<float>(videoPortPtr->Height());
                return Rendering::Perspective(m_fov * Math::Deg2RadF, aspectRatio, m_nearPlane, m_farPlane);
            }

            case CameraType::Orthographic:
                return Math::Matrix4F::Identity();
                // return Rendering::Orthographic();

            default:
                return Math::Matrix4F::Identity();
        }
    }

    void Camera::Update()
    {
        if (m_timer.expired())
            return;

        auto timer = m_timer.lock();
        float velocity = m_moveSpeed * timer->DeltaTime() * 0.001f;
        if (Input::GetKey(Input::KeyCode::W))
            m_position = m_position + velocity * m_front;
        if (Input::GetKey(Input::KeyCode::S))
            m_position = m_position - velocity * m_front;
        if (Input::GetKey(Input::KeyCode::A))
            m_position = m_position - velocity * m_right;
        if (Input::GetKey(Input::KeyCode::D))
            m_position = m_position + velocity * m_right;
        if (Input::GetKey(Input::KeyCode::Q))
            m_position = m_position - velocity * m_worldUp;
        if (Input::GetKey(Input::KeyCode::E))
            m_position = m_position + velocity * m_worldUp;

        if (Input::GetKeyDown(Input::KeyCode::RightMouseButton))
        {
            m_lastMouseX = Input::MousePositionX();
            m_lastMouseY = Input::MousePositionY();
        }

        if (Input::GetKey(Input::KeyCode::RightMouseButton))
        {
            const float xOffset = static_cast<float>(Input::MousePositionX() - m_lastMouseX) * m_mouseSensitivity;
            const float yOffset = static_cast<float>(Input::MousePositionY() - m_lastMouseY) * m_mouseSensitivity;
            m_lastMouseX = Input::MousePositionX();
            m_lastMouseY = Input::MousePositionY();

            m_yaw -= xOffset;
            m_pitch = std::clamp(m_pitch - yOffset, -89.0f, 89.0f);

            UpdateCameraVectors();
        }

        m_fov = std::clamp(m_fov - static_cast<float>(Input::MouseWheelDelta()), 1.0f, 45.0f);
    }

    void Camera::UpdateCameraVectors()
    {
        m_front.X() = -std::cos(m_pitch * Math::Deg2RadF) * std::sin(m_yaw * Math::Deg2RadF);
        m_front.Y() = std::sin(m_pitch * Math::Deg2RadF);
        m_front.Z() = -std::cos(m_pitch * Math::Deg2RadF) * std::cos(m_yaw * Math::Deg2RadF);
        m_right = m_front.Cross(m_worldUp);
    }
}
