/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Core.hpp"

namespace TG::Math::Geometry
{
    class Sphere
    {
    public:
        Sphere() = default;
        Sphere(const Vector3f& center, float radius);

        [[nodiscard]] Vector3f Center() const;
        [[nodiscard]] float Radius() const;

        [[nodiscard]] bool Contains(const Vector3f& point) const;

    private:
        Vector3f m_center;
        float m_radius{ 1.0f };
    };
}
