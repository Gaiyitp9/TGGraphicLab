/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Math/Geometry/Intersection/Intersection.h"

namespace TG::Math::Geometry
{
    bool RaySphereIntersection(const Ray &ray, const Sphere &sphere, float &tMin, float &tMax)
    {
        const Vector3f oc = ray.Origin() - sphere.Center();
        const float a = ray.Direction().SquaredNorm();
        const float halfB = oc.Dot(ray.Direction());
        const float c = oc.SquaredNorm() - sphere.Radius() * sphere.Radius();

        if (const float discriminant = halfB * halfB - a * c; discriminant < 0)
        {
            tMin = tMax = std::numeric_limits<double>::infinity();
            return false;
        }
        else
        {
            const float sqrtDiscriminant = std::sqrt(discriminant);
            tMin = (-halfB - sqrtDiscriminant) / a;
            tMax = (-halfB + sqrtDiscriminant) / a;
            return true;
        }
    }
}
