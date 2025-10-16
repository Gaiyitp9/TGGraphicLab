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
        Ray(const Math::Vector3d& origin, const Math::Vector3d& direction);

        [[nodiscard]] Math::Vector3f At(double t) const;

        [[nodiscard]] Math::Vector3d Origin() const { return m_origin; }
        [[nodiscard]] Math::Vector3d Direction() const { return m_direction; }

    private:
        Math::Vector3d m_origin;
        Math::Vector3d m_direction;
    };
}
