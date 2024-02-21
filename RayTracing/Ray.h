#pragma once

#include "Vec3.h"

class Ray
{
public:
    Ray() {}
    Ray(const Point3& origin, const Vec3& direction) : m_origin(origin), m_dir(direction) {}

    Point3 Origin() const { return m_origin; }
    Vec3 Direction() const { return m_dir; }

    Point3 At(double t)
    {
        return m_origin + t * m_dir;
    }

private:
    Point3 m_origin;
    Vec3 m_dir;
};
