/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Geometry/Shape/Ray.h"
#include "Math/Geometry/Shape/Sphere.h"

namespace TG::Math::Geometry
{
    bool RaySphereIntersection(const Ray& ray, const Sphere& sphere, float& tMin, float& tMax);
}
