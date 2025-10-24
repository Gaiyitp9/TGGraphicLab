/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Geometry/Shape/Ray.h"
#include "Math/Geometry/Shape/Sphere.h"
#include "Math/Geometry/Shape/Plane.h"

namespace TG::Math::Geometry::Intersection
{
    bool RaySphere(const Ray& ray, const Sphere& sphere, float& tMin, float& tMax);
    bool RayPlane(const Ray& ray, const Plane& plane, float& t);
}
