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
        Math::Vector3f corners[8];
    };

    class Camera
    {
    public:
        Camera(const std::weak_ptr<IDefaultVideoPort>& videoPort, const std::weak_ptr<ITimer>& timer);
        ~Camera();

        [[nodiscard]] Math::Matrix4f ViewMatrix() const;
        [[nodiscard]] Math::Matrix4f ProjectionMatrix() const;
        [[nodiscard]] Frustum ViewFrustum() const;
        [[nodiscard]] float AspectRatio() const;

        void Update();

        CameraType cameraType{ CameraType::Perspective };

        Math::Vector3f position{ 0.0f, 1.0f, 4.0f };
        float fov{ 45.0 };
        float nearPlane{ 0.1f };
        float farPlane{ 100.0f };
        float orthoWidth{ 3.0f };

        Math::Vector3f front;
        Math::Vector3f right;
        Math::Vector3f up;

    private:
        void UpdateCameraVectors();

        std::weak_ptr<IDefaultVideoPort> m_videoPort;
        std::weak_ptr<ITimer> m_timer;

        Math::Vector3f m_worldUp{ 0.0f, 1.0f, 0.0f };

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
