/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Math/Geometry/AABB.h"

namespace TG::Math::Geometry
{
    AABB::AABB(const Vector3f &min, const Vector3f &max)
        : m_min(min), m_max(max)
    {

    }

    Vector3f AABB::Center() const
    {
        return (m_min + m_max) * 0.5f;
    }

    Vector3f AABB::Extent() const
    {
        return (m_max - m_min) * 0.5f;
    }

    void AABB::Expand(const Vector3f& point)
    {
        m_min.X() = std::min(m_min.X(), point.X());
        m_min.Y() = std::min(m_min.Y(), point.Y());
        m_min.Z() = std::min(m_min.Z(), point.Z());

        m_max.X() = std::max(m_max.X(), point.X());
        m_max.Y() = std::max(m_max.Y(), point.Y());
        m_max.Z() = std::max(m_max.Z(), point.Z());
    }

    bool AABB::Contains(const Vector3f &point) const
    {
        return point.X() > m_min.X() && point.X() < m_max.X() &&
            point.Y() > m_min.Y() && point.Y() < m_max.Y() &&
            point.Z() > m_min.Z() && point.Z() < m_max.Z();
    }
}
