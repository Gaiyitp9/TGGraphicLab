#pragma once

#include "Hittable.h"

class Sphere : public Hittable
{
public:
    Sphere() = default;
    Sphere(const Point3& c, double r) : center(c), radius(r) {}

    bool Hit(const Ray& ray, const Interval& rayInterval, HitRecord& record) const override;

    Point3 center;
    double radius = 0.5;
};
