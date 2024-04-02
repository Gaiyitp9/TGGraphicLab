#pragma once

#include "Hittable.h"
#include "Color.h"

class Material
{
public:
    virtual ~Material() = default;

    virtual bool Scatter(const Ray& r, const HitRecord& record, Color& attenuation, Ray& scattered) const = 0;
};

class Lambert : public Material
{
public:
    explicit Lambert(const Color& a) : m_albedo(a) {}

    bool Scatter(const Ray& r, const HitRecord& record, Color& attenuation, Ray& scattered) const override
    {
        Vec3 scatterDirection = CosineSampleHemisphere(record.normal, record.tangent, record.binormal);
        scattered = Ray(record.p, scatterDirection);
        attenuation = m_albedo;
        return true;
    }

private:
    Color m_albedo;
};

class Metal : public Material
{
public:
    explicit Metal(const Color& a, double fuzz) : m_albedo(a), m_fuzz(fuzz < 1 ? fuzz : 1) {}

    bool Scatter(const Ray& r, const HitRecord& record, Color& attenuation, Ray& scattered) const override
    {
        Vec3 reflected = Reflect(r.dir.Normalized(), record.normal);
        scattered = Ray(record.p, reflected + m_fuzz * RandomOnUnitSphere());
        attenuation = m_albedo;
        return (Dot(scattered.dir, record.normal) > 0);
    }

private:
    Color m_albedo;
    double m_fuzz;
};

class Dielectric : public Material
{
public:
    explicit Dielectric(double indexOfRefraction) : ior(indexOfRefraction) {}

    bool Scatter(const Ray& r, const HitRecord& record, Color& attenuation, Ray& scattered) const override
    {
        attenuation = Color(1.0, 1.0, 1.0);
        double refractionRatio = record.frontFace ? (1.0 / ior) : ior;

        Vec3 unitDirection = r.dir.Normalized();
        double cosTheta = Dot(-unitDirection, record.normal);
        double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

        Vec3 direction;
        // 发生全反射
        if (refractionRatio * sinTheta > 1.0 || Reflectance(cosTheta, refractionRatio) > RandomDouble())
            direction = Reflect(unitDirection, record.normal);
        else
            direction = Refract(unitDirection, record.normal, refractionRatio);

        scattered = Ray(record.p, direction);
        return true;
    }

private:
    double ior;

    static double Reflectance(double cosine, double refIdx)
    {
        // 菲涅耳反射的Schlick近似
        double r0 = (1 - refIdx) / (1 + refIdx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow(1 - cosine, 5);
    }
};
