#pragma once

#include "Ray.h"
#include "Interval.h"

class Material;

struct HitRecord
{
    Point3  p;
    Vec3    normal;
    double  t = 0;
    bool    frontFace = true;
    Vec3    tangent;
    Vec3    binormal;
    std::shared_ptr<Material> material;

    void SetFaceNormal(const Ray& r, const Vec3& outwardNormal)
    {
        frontFace = Dot(r.dir, outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
        double d = Dot(normal, p);
        if (std::abs(normal.X()) > Epsilon)
        {
            tangent.SetY(p.Y() - 1.0);
            tangent.SetZ(p.Z() - 1.0);
            tangent.SetX((d - normal.Y() * tangent.Y() - normal.Z() * tangent.Z()) / normal.X());
        }
        else if (std::abs(normal.Y()) > Epsilon)
        {
            tangent.SetX(p.X() - 1.0);
            tangent.SetZ(p.Z() - 1.0);
            tangent.SetY((d - normal.X() * tangent.X() - normal.Z() * tangent.Z()) / normal.Y());
        }
        else
        {
            tangent.SetX(p.X() - 1.0);
            tangent.SetY(p.Y() - 1.0);
            tangent.SetZ((d - normal.X() * tangent.X() - normal.Y() * tangent.Y()) / normal.Z());
        }
        tangent.Normalize();
        binormal = Cross(tangent, normal);
    }
};

class Hittable
{
public:
    Hittable() = default;

    virtual bool Hit(const Ray& ray, const Interval& rayInterval, HitRecord& record) const = 0;
};
