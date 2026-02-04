/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Math/Geometry/Intersection.h"

namespace TG::Math::Geometry::Intersection
{
    bool RaySphere(const Ray& ray, const Sphere& sphere, float& tMin, float& tMax)
    {
        // 光线与球相交时，交点在球面上
        // 设O为光线起点，D为光线方向，C为球心，r为半径，P为光线和球的交点，
        // 对于P，满足
        // 1) (P-C)^2=r^2
        // 2) P=O+tD
        // 联立两个等式，得到
        // D^2*t^2 + 2*(O-C)*D*t+(O-C)^2-r^2=0
        // 求解二元一次方程得到t，再带入射线表达式得到交点
        const Vector3f oc = ray.Origin() - sphere.Center();
        const float a = ray.Direction().SquaredNorm();
        const float halfB = oc.Dot(ray.Direction());
        const float c = oc.SquaredNorm() - sphere.Radius() * sphere.Radius();

        if (const float discriminant = halfB * halfB - a * c; discriminant < 0)
        {
            tMin = tMax = std::numeric_limits<float>::infinity();
            return false;
        }
        else
        {
            const float sqrtDiscriminant = std::sqrt(discriminant);
            tMin = (-halfB - sqrtDiscriminant) / a;
            tMax = (-halfB + sqrtDiscriminant) / a;
            return true;
        }
    }

    bool RayConvexPolygon(const Ray& ray, const ConvexPolygon& convex, float& t)
    {
        const Plane& plane = convex.GetPlane();
        const float temp = -plane.SignedDistance(ray.Origin()) / plane.Normal().Dot(ray.Direction());

        if (!convex.Contains(ray.At(temp)))
            return false;

        t = temp;
        return true;
    }
}
