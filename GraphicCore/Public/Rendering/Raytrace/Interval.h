/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <limits>

namespace TG::Rendering
{

    class Interval
    {
    public:
        Interval() = default;
        Interval(float min, float max);

        [[nodiscard]] float Size() const;
        [[nodiscard]] bool Contains(float x) const;
        [[nodiscard]] bool Surrounds(float x) const;
        [[nodiscard]] float Clamp(float x) const;

    private:
        float m_min{ std::numeric_limits<float>::infinity() };
        float m_max{ std::numeric_limits<float>::infinity() };
    };
}
