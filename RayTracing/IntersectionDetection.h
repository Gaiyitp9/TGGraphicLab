#pragma once

#include "Sphere.h"

namespace IntersectionUtil
{
    bool RaySphereIntersection(const Ray& ray, const Sphere& sphere, double& tMin, double& tMax);
}
