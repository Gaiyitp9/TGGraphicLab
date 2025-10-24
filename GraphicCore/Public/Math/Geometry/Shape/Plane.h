/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Core.hpp"

namespace TG::Math::Geometry
{
    class Plane
    {
    public:
        Plane() = default;
        Plane(const Vector3f& point, const Vector3f& normal);
        Plane(float a, float b, float c, float d);

    private:
        float m_a, m_b, m_c, m_d;
    };
}
