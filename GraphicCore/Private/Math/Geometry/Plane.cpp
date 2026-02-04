/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Math/Geometry/Plane.h"

namespace TG::Math::Geometry
{
    Plane::Plane(const Vector3f& point, const Vector3f& normal)
        : m_normal{ normal.Normalized() }, m_d{ -point.Dot(normal) }
    {

    }

    Vector3f Plane::Normal() const
    {
        return m_normal;
    }

    float Plane::SignedDistance(const Vector3f &point) const
    {
        return point.Dot(m_normal) + m_d;
    }

    float Plane::Distance(const Vector3f &point) const
    {
        return std::fabs(SignedDistance(point));
    }
}
