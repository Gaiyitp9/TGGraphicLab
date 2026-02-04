/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Rendering/RayTracing/Scene.h"
#include "Math/Geometry/Sphere.h"
#include "Math/Geometry/Intersection.h"
#include "Rendering/Color/StandardColors.h"
#include "Rendering/RayTracing/Material.h"

namespace TG::Rendering::RayTracing
{
    Scene::Scene() = default;

    Color Scene::RayColor(const Math::Geometry::Ray &ray, unsigned int depth) const
    {
        if (depth <= 0)
            return Black;

        if (HitRecord record; Hit(ray, Interval(0.001f, std::numeric_limits<float>::max()), record))
        {
            Math::Geometry::Ray scattered;
            Color attenuation;
            if (record.material->Scatter(ray, record, attenuation, scattered))
                return attenuation * RayColor(scattered, depth - 1);
            return Black;
        }

        Math::Vector3f unitDirection = ray.Direction().Normalized();
        const float a = 0.5f * (unitDirection.Y() + 1.0f);
        return (1 - a) * White + a * Color(0.5f, 0.7f, 1.0f);
    }

    void Scene::Add(const std::shared_ptr<Hittable>& object)
    {
        m_objects.push_back(object);
    }

    void Scene::Clear()
    {
        m_objects.clear();
    }

    bool Scene::Hit(const Math::Geometry::Ray &ray, const Interval &rayInterval, HitRecord &record) const
    {
        HitRecord recordTemp;
        bool hitAnything = false;
        Interval interval = rayInterval;

        for (const std::shared_ptr<Hittable>& object: m_objects)
        {
            if (object->Hit(ray, interval, recordTemp))
            {
                hitAnything = true;
                interval.max = recordTemp.t;
                record = recordTemp;
            }
        }

        return hitAnything;
    }
}
