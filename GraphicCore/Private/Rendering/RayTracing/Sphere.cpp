/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Rendering/RayTracing/Sphere.h"
#include "Math/Geometry/Intersection.h"

namespace TG::Rendering::RayTracing
{
    Sphere::Sphere(const Math::Vector3f& center, float radius) : m_sphere(center, radius) {}

    bool Sphere::Hit(const Math::Geometry::Ray& ray, const Interval& rayInterval, HitRecord& record) const
    {
        float tMin, tMax;
        if (!Math::Geometry::Intersection::RaySphere(ray, m_sphere, tMin, tMax))
            return false;

        if (rayInterval.Surrounds(tMin))
        {
            record.t = tMin;
            record.position = ray.At(tMin);
            const Math::Vector3f outwardNormal = (record.position - m_sphere.Center()) / m_sphere.Radius();
            record.SetFaceNormal(ray, outwardNormal);
            return true;
        }
        if (rayInterval.Surrounds(tMax))
        {
            record.t = tMax;
            record.position = ray.At(tMax);
            const Math::Vector3f outwardNormal = (record.position - m_sphere.Center()) / m_sphere.Radius();
            record.SetFaceNormal(ray, outwardNormal);
            return true;
        }

        return false;
    }
}
