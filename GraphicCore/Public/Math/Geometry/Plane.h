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

        [[nodiscard]] Vector3f Normal() const;
        [[nodiscard]] float SignedDistance(const Vector3f& point) const;
        [[nodiscard]] float Distance(const Vector3f& point) const;

    private:
        Vector3f m_normal{ 0, 1, 0 };
        float m_d{ 0 };
    };
}
