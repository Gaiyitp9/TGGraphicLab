/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Rendering/RayTracing/Interval.h"

namespace TG::Rendering
{
    Interval::Interval(float min, float max)
        : min(min), max(max)
    {

    }

    float Interval::Size() const
    {
        return max - min;
    }

    bool Interval::Contains(float x) const
    {
        return min <= x && x <= max;
    }

    bool Interval::Surrounds(float x) const
    {
        return min < x && x < max;
    }

    float Interval::Clamp(float x) const
    {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }
}
