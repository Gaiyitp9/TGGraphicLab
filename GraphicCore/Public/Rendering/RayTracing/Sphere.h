/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/RayTracing/Hittable.h"
#include "Math/Geometry/Sphere.h"

namespace TG::Rendering::RayTracing
{
    class Sphere final : public Hittable
    {
    public:
        Sphere() = default;
        Sphere(const Math::Vector3f& center, float radius);
        ~Sphere() override = default;

        bool Hit(const Math::Geometry::Ray& ray, const Interval& rayInterval, HitRecord& record) const override;

    private:
        Math::Geometry::Sphere m_sphere;
    };
}
