#pragma once

#include "Ray.h"
#include "Interval.h"

struct HitRecord
{
    Point3  p;
    Vec3    normal;
    double  t = 0;
    bool    frontFace = true;

    void SetFaceNormal(const Ray& r, const Vec3& outwardNormal)
    {
        frontFace = Dot(r.dir, outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class Hittable
{
public:
    Hittable() = default;

    virtual bool Hit(const Ray& ray, const Interval& rayInterval, HitRecord& record) const = 0;
};
