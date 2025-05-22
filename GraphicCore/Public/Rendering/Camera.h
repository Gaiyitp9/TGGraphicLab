/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Core.hpp"
#include "Base/CommonInterfaces.h"

namespace TG
{
    enum class CameraType
    {
        Perspective,
        Orthographic,
    };

    class Camera
    {
    public:
        Camera(const std::weak_ptr<IDefaultVideoPort>& videoPort, const std::weak_ptr<ITimer>& timer);
        ~Camera();

        [[nodiscard]] Math::Matrix4F ViewMatrix() const;
        [[nodiscard]] Math::Matrix4F ProjectionMatrix() const;

        void Update();

    private:
        void UpdateCameraVectors();

        std::weak_ptr<IDefaultVideoPort> m_videoPort;
        std::weak_ptr<ITimer> m_timer;

        CameraType m_cameraType{ CameraType::Perspective };

        float m_fov{ 45.0 };
        float m_nearPlane{ 0.1f };
        float m_farPlane{ 100.0f };

        float m_orthoWidth{ 3.0f };

        Math::Vector3F m_position{ 0.0f, 0.0f, 3.0f };
        Math::Vector3F m_front{ 0.0f, 0.0f, -1.0f };
        Math::Vector3F m_right{ 1.0f, 0.0f, 0.0f };
        Math::Vector3F m_worldUp{ 0.0f, 1.0f, 0.0f };

        float m_yaw{ 0.0f };
        float m_pitch{ 0.0f };
        short m_lastMouseX{ 0 };
        short m_lastMouseY{ 0 };

        float m_moveSpeed{ 2.5f };
        float m_mouseSensitivity{ 0.1f };
    };
}
