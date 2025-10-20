/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Core.hpp"

namespace TG::Math::Geometry
{
    class Ray
    {
    public:
        Ray() = default;
        Ray(const Vector3f& origin, const Vector3f& direction);

        [[nodiscard]] Vector3f Origin() const;
        [[nodiscard]] Vector3f Direction() const;

        [[nodiscard]] Vector3f At(float t) const;

    private:
        Vector3f m_origin;
        Vector3f m_direction{ 1.0f, 0.0f, 0.0f };
    };
}
