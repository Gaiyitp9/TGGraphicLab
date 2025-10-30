/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Core.hpp"

namespace TG::Math::Geometry
{
    /**
     * \brief 轴对称包围盒(Axis Aligned Bounding Box)
     */
    class AABB
    {
    public:
        AABB() = default;
        AABB(const Vector3f& min, const Vector3f& max);

        [[nodiscard]] Vector3f Center() const;
        [[nodiscard]] Vector3f Extent() const;

        void Expand(const Vector3f& point);
        [[nodiscard]] bool Contains(const Vector3f& point) const;

    private:
        Vector3f m_min;
        Vector3f m_max;
    };
}
