/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Math/Geometry/AABB.h"

namespace TG::Math::Geometry
{
    AABB::AABB(const Vector3f& min, const Vector3f& max) : m_min(min), m_max(max) {}

    AABB AABB::FromCenterAndExtent(const Vector3f& center, const Vector3f& extent)
    {
        AABB aabb;
        aabb.m_min = center - extent;
        aabb.m_max = center + extent;
        return aabb;
    }

    AABB AABB::FromOriginAndSize(const Vector3f& origin, const Vector3f& size)
    {
        AABB aabb;
        aabb.m_min = origin;
        aabb.m_max = origin + size;
        return aabb;
    }
    
    Vector3<AABB::T> AABB::Min() const
    {
        return m_min;
    }
    Vector3<AABB::T> AABB::Max() const
    {
        return m_max;
    }
    Vector3<AABB::T> AABB::Center() const
    {
        return (m_min + m_max) * static_cast<T>(0.5);
    }
    Vector3<AABB::T> AABB::Extent() const
    {
        return (m_max - m_min) * static_cast<T>(0.5);
    }
    Vector3<AABB::T> AABB::Size() const
    {
        return m_max - m_min;
    }

    void AABB::Expand(const Vector3<T>& p)
    {
        m_min.X() = std::min(m_min.X(), p.X());
        m_min.Y() = std::min(m_min.Y(), p.Y());
        m_min.Z() = std::min(m_min.Z(), p.Z());

        m_max.X() = std::max(m_max.X(), p.X());
        m_max.Y() = std::max(m_max.Y(), p.Y());
        m_max.Z() = std::max(m_max.Z(), p.Z());
    }
}
