#pragma once

#include "Vec3.h"

struct Ray
{
    Point3 origin;
    Vec3 dir;

    [[nodiscard]] Point3 At(double t) const
    {
        return origin + t * dir;
    }
};
