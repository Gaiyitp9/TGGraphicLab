/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Rendering/Camera.h"
#include "Rendering/ViewProjectionMatrix.hpp"

namespace TG
{
    Camera::Camera(const std::weak_ptr<IDefaultVideoPort>& videoPort, const std::weak_ptr<ITimer>& timer)
        : m_videoPort(videoPort), m_timer(timer)
    {
        if (m_videoPort.expired() || m_timer.expired())
            throw BaseException::Create("Interfaces are not valid");

        auto videoPortPtr = m_videoPort.lock();

        m_aspectRatio = static_cast<float>(videoPortPtr->Width()) / static_cast<float>(videoPortPtr->Height());
    }

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
                return Rendering::Perspective(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);

            case CameraType::Orthographic:
                return Math::Matrix4F::Identity();
                // return Rendering::Orthographic();

            default:
                return Math::Matrix4F::Identity();
        }
    }
}
