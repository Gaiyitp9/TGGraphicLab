/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Precision.hpp"
#include "Math/Core.hpp"

namespace TG::Math::Geometry
{
    /**
     * \brief 轴对称包围盒(Axis Aligned Bounding Box)
     */
    class AABB
    {
        using T = FloatType;

    public:
        AABB() = default;
        AABB(const Vector3f& min, const Vector3f& max);
        static AABB FromCenterAndExtent(const Vector3f& center, const Vector3f& extent);
        static AABB FromOriginAndSize(const Vector3f& origin, const Vector3f& size);

        [[nodiscard]] Vector3<T> Min() const;
        [[nodiscard]] Vector3<T> Max() const;
        [[nodiscard]] Vector3<T> Center() const;
        [[nodiscard]] Vector3<T> Extent() const;
        [[nodiscard]] Vector3<T> Size() const;

        void Expand(const Vector3<T>& p);

    private:
        Vector3<T> m_min;
        Vector3<T> m_max;
    };
}
