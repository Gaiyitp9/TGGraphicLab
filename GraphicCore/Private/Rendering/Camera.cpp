/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Rendering/Camera.h"
#include "Rendering/ViewProjectionMatrix.hpp"

namespace TG
{
    Camera::Camera(const IDefaultVideoPort& videoPort) : m_videoPort(videoPort)
    {
        m_aspectRatio = static_cast<float>(m_videoPort.Width()) / static_cast<float>(m_videoPort.Height());
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
