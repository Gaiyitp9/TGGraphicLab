/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Geometry/Ray.h"
#include "Sphere.h"
#include "ConvexPolygon.h"

namespace TG::Math::Geometry::Intersection
{
    bool RaySphere(const Ray& ray, const Sphere& sphere, float& tMin, float& tMax);
    bool RayConvexPolygon(const Ray& ray, const ConvexPolygon& convex, float& t);
}
