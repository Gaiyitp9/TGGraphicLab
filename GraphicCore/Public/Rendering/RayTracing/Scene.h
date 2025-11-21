/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/Color/Color.h"
#include "Rendering/RayTracing/Hittable.h"
#include <vector>

namespace TG::Rendering::RayTracing
{
    class Scene
    {
    public:
        Scene();

        [[nodiscard]] Color RayColor(const Math::Geometry::Ray& ray, unsigned int depth) const;

        void Add(const std::shared_ptr<Hittable>& object);
        void Clear();
        bool Hit(const Math::Geometry::Ray& ray, const Interval& rayInterval, HitRecord& record) const;

    private:
        std::vector<std::shared_ptr<Hittable>> m_objects;
    };
}
