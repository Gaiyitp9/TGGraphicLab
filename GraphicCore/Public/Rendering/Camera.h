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

    struct Frustum
    {
        Math::Vector3F corners[8];
    };

    class Camera
    {
    public:
        Camera(const std::weak_ptr<IDefaultVideoPort>& videoPort, const std::weak_ptr<ITimer>& timer);
        ~Camera();

        [[nodiscard]] Math::Matrix4F ViewMatrix() const;
        [[nodiscard]] Math::Matrix4F ProjectionMatrix() const;
        [[nodiscard]] Frustum ViewFrustum() const;

        void Update();

        Math::Vector3F position{ 0.0f, 1.0f, 4.0f };

    private:
        void UpdateCameraVectors();

        std::weak_ptr<IDefaultVideoPort> m_videoPort;
        std::weak_ptr<ITimer> m_timer;

        CameraType m_cameraType{ CameraType::Perspective };

        float m_fov{ 45.0 };
        float m_nearPlane{ 0.1f };
        float m_farPlane{ 100.0f };

        float m_orthoWidth{ 3.0f };

        Math::Vector3F m_front;
        Math::Vector3F m_right;
        Math::Vector3F m_up;
        Math::Vector3F m_worldUp{ 0.0f, 1.0f, 0.0f };

        float m_yaw{ 0.0f };
        float m_pitch{ -5.0f };
        short m_clickMouseX{ 0 };
        short m_clickMouseY{ 0 };
        short m_lastMouseX{ 0 };
        short m_lastMouseY{ 0 };

        float m_moveSpeed{ 5.0f };
        float m_mouseSensitivity{ 0.1f };
    };
}
