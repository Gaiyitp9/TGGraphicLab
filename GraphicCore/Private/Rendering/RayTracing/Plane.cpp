/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Rendering/RayTracing/Plane.h"
#include "Math/Geometry/Intersection.h"

namespace TG::Rendering::RayTracing
{
    Plane::Plane(const Math::Vector3f& normal, const std::vector<Math::Vector3f>& points,
        const std::shared_ptr<Material>& material) : m_plane(points[0], normal),
            m_convex(m_plane, points), m_material(material)
    {}

    bool Plane::Hit(const Math::Geometry::Ray &ray, const Interval &rayInterval, HitRecord &record) const
    {
        float t;
        if (!Math::Geometry::Intersection::RayConvexPolygon(ray, m_convex, t))
            return false;

        if (rayInterval.Surrounds(t))
        {
            record.t = t;
            record.position = ray.At(t);
            record.material = m_material;
            record.SetFaceNormal(ray, m_plane.Normal());
            return true;
        }

        return false;
    }
}
