/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Rendering/RayTracing/Material.h"
#include "Math/Random.hpp"
#include "Math/Common.hpp"

namespace TG::Rendering::RayTracing
{
    Lambert::Lambert(const Color &a) : m_albedo(a)
    {
        
    }

    bool Lambert::Scatter(const Math::Geometry::Ray&, const HitRecord& record, Color& attenuation,
        Math::Geometry::Ray& scattered) const
     {
        Math::Vector3f scatterDirection = Math::g_random.CosineWeightedSampleOnHemisphere();
        // 采样变量从法线空间变换到世界空间
        scatterDirection = scatterDirection.X() * record.binormal + scatterDirection.Y() * record.tangent +
            scatterDirection.Z() * record.normal;
        scattered = Math::Geometry::Ray(record.position, scatterDirection);
        attenuation = m_albedo;
        return true;
    }

    Metal::Metal(const Color& a, float fuzz) : m_albedo(a), m_fuzz(fuzz < 1 ? fuzz : 1)
    {

    }

    bool Metal::Scatter(const Math::Geometry::Ray& r, const HitRecord& record, Color& attenuation,
        Math::Geometry::Ray& scattered) const
    {
        Math::Vector3f reflected = Reflect(r.Direction().Normalized(), record.normal);
        scattered = Math::Geometry::Ray(record.position, reflected + m_fuzz *
            Math::g_random.UniformSampleOnHemisphere());
        attenuation = m_albedo;
        return scattered.Direction().Dot(record.normal) > 0;
    }

    Dielectric::Dielectric(float indexOfRefraction) : m_indexOfRefraction(indexOfRefraction)
    {

    }

    bool Dielectric::Scatter(const Math::Geometry::Ray& r, const HitRecord& record, Color& attenuation,
            Math::Geometry::Ray& scattered) const
    {
        attenuation = Color(1.0, 1.0, 1.0);
        float refractionRatio = record.frontFace ? 1.0f / m_indexOfRefraction : m_indexOfRefraction;

        Math::Vector3f unitDirection = r.Direction().Normalized();
        float cosTheta = -unitDirection.Dot(record.normal);
        float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);

        Math::Vector3f direction;
        // 发生全反射
        if (refractionRatio * sinTheta > 1.0f || Reflectance(cosTheta, refractionRatio) > Math::g_random.Float())
            direction = Reflect(unitDirection, record.normal);
        else
            direction = Refract(unitDirection, record.normal, refractionRatio);

        scattered = Math::Geometry::Ray(record.position, direction);
        return true;
    }
}
