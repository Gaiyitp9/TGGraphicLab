#include "Sphere.h"
#include "IntersectionDetection.h"

bool Sphere::Hit(const Ray &ray, const Interval& rayInterval, HitRecord &record) const
{
    double tMin, tMax;
    if (!IntersectionUtil::RaySphereIntersection(ray, *this, tMin, tMax))
        return false;

    if (rayInterval.Surrounds(tMin))
    {
        record.t = tMin;
        record.p = ray.At(tMin);
        Vec3 outwardNormal = (record.p - center) / radius;
        record.SetFaceNormal(ray, outwardNormal);
        record.material = mat;
        return true;
    }
    if (rayInterval.Surrounds(tMax))
    {
        record.t = tMax;
        record.p = ray.At(tMax);
        Vec3 outwardNormal = (record.p - center) / radius;
        record.SetFaceNormal(ray, outwardNormal);
        record.material = mat;
        return true;
    }

    return false;
}
