/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/RayTracing/Hittable.h"
#include "Math/Geometry/ConvexPolygon.h"

namespace TG::Rendering::RayTracing
{
    class Plane : public Hittable
    {
    public:
        Plane() = default;
        Plane(const Math::Vector3f& normal, const std::vector<Math::Vector3f>& points);
        ~Plane() override = default;

        bool Hit(const Math::Geometry::Ray &ray, const Interval &rayInterval, HitRecord &record) const override;

    private:
        Math::Geometry::Plane m_plane{ { 0.0f, 0.01f, 0.0f }, { 0.0f, 1.0f, 0.0f } };
        Math::Geometry::ConvexPolygon m_convex{
            m_plane,
            {
                    { -10.0f, 0.01f, -20.0f },
                    {  10.0f, 0.01f, -20.0f },
                    {  10.0f, 0.01f,  0.0f },
                    { -10.0f, 0.01f,  0.0f },
            }
        };
    };
}
