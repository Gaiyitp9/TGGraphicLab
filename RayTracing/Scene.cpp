#include "Scene.h"

Scene::Scene() = default;

Color Scene::RayColor(const Ray &ray, unsigned int depth) const
{
    if (depth <= 0)
        return {0, 0, 0};

    HitRecord record;
    if (Hit(ray, Interval(0.001, std::numeric_limits<double>::max()), record))
    {
//        Vec3 direction = RandomOnHemisphere(record.normal, record.tangent, record.binormal);
        Vec3 direction = CosineSampleHemisphere(record.normal, record.tangent, record.binormal);
        return 0.5 * RayColor({record.p, direction}, depth - 1);
    }

    Vec3 unitDirection = ray.dir.Normalized();
    double a = 0.5 * (unitDirection.Y() + 1.0);
    return (1 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

void Scene::Add(const std::shared_ptr<Hittable>& object)
{
    m_objects.push_back(object);
}

void Scene::Clear()
{
    m_objects.clear();
}

bool Scene::Hit(const Ray &ray, const Interval& rayInterval, HitRecord &record) const
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
