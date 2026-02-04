/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Math/Geometry/Sphere.h"

namespace TG::Math::Geometry
{
    Sphere::Sphere(const Vector3f &center, float radius)
        : m_center(center), m_radius(radius)
    {

    }

    Vector3f Sphere::Center() const
    {
        return m_center;
    }

    float Sphere::Radius() const
    {
        return m_radius;
    }

    bool Sphere::Contains(const Vector3f& point) const
    {
        return (point - m_center).SquaredNorm() < m_radius * m_radius;
    }
}
