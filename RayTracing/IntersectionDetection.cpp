#include "IntersectionDetection.h"

namespace IntersectionUtil
{
    bool RaySphereIntersection(const Ray& ray, const Sphere& sphere, double& tMin, double& tMax)
    {
        Vec3 oc = ray.origin - sphere.center;
        double a = Dot(ray.dir, ray.dir);
        double halfB = Dot(oc, ray.dir);
        double c = oc.LengthSquared() - sphere.radius * sphere.radius;
        double discriminant = halfB * halfB - a * c;

        if (discriminant < 0)
        {
            tMin = tMax = std::numeric_limits<double>::infinity();
            return false;
        }
        else
        {
            double sqrtDiscriminant = std::sqrt(discriminant);
            tMin = (-halfB - sqrtDiscriminant) / a;
            tMax = (-halfB + sqrtDiscriminant) / a;
            return true;
        }
    }
}
