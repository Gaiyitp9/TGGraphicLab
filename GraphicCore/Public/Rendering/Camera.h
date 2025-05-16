/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Core.hpp"

namespace TG
{
    class Camera
    {
    public:
        Camera();
        ~Camera();

        [[nodiscard]] Math::Matrix4F ViewMatrix() const;
        [[nodiscard]] Math::Matrix4F ProjectionMatrix() const;

    private:
        // glm::vec3 m_position;
        // glm::vec3 m_front;
        // glm::vec3 m_up;
        Math::Vector3F m_worldUp{ 0.0f, 1.0f, 0.0f };
        float m_yaw;
        float m_pitch;

        float m_moveSpeed;
        float m_mouseSensitivity;
        float m_fov;
    };
}
