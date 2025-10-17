/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Core.hpp"

namespace TG
{
    class Ray
    {
    public:
        Ray() = default;
        Ray(const Math::Vector3f& origin, const Math::Vector3f& direction);

        [[nodiscard]] Math::Vector3f At(float t) const;

        [[nodiscard]] Math::Vector3f Origin() const { return m_origin; }
        [[nodiscard]] Math::Vector3f Direction() const { return m_direction; }

    private:
        Math::Vector3f m_origin;
        Math::Vector3f m_direction;
    };
}
