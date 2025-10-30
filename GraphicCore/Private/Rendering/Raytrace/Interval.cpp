/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Rendering/Raytrace/Interval.h"

namespace TG::Rendering
{
    Interval::Interval(float min, float max)
        : m_min(min), m_max(max)
    {

    }

    float Interval::Size() const
    {
        return m_max - m_min;
    }

    bool Interval::Contains(float x) const
    {
        return m_min <= x && x <= m_max;
    }

    bool Interval::Surrounds(float x) const
    {
        return m_min < x && x < m_max;
    }

    float Interval::Clamp(float x) const
    {
        if (x < m_min) return m_min;
        if (x > m_max) return m_max;
        return x;
    }
}
