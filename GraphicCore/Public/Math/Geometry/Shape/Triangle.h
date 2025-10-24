/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Core.hpp"

namespace TG::Math::Geometry
{
    class Triangle
    {
    public:
        Triangle() = default;
        Triangle(const Vector3f& point0, const Vector3f& point1, const Vector3f& point2);

        [[nodiscard]] bool Contains(const Vector3f& point) const;

    private:
        Vector3f m_point0;
        Vector3f m_point1;
        Vector3f m_point2;
    };
}
