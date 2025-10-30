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
        Triangle(const Vector3f& pointA, const Vector3f& pointB, const Vector3f& pointC);

        [[nodiscard]] bool Contains(const Vector3f& point) const;

    private:
        Vector3f m_pointA;
        Vector3f m_pointB;
        Vector3f m_pointC;
    };
}
