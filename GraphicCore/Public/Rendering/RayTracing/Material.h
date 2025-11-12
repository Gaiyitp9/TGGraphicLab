/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Geometry/Ray.h"
#include "Rendering/RayTracing/Hittable.h"
#include "Rendering/Color/Color.h"

namespace TG::Rendering::RayTracing
{
    class Material
    {
    public:
        virtual ~Material() = default;

        virtual bool Scatter(const Math::Geometry::Ray& r, const HitRecord& record, Color& attenuation,
            Math::Geometry::Ray& scattered) const = 0;
    };

    class Lambert final : public Material
    {
    public:
        explicit Lambert(const Color& a);

        bool Scatter(const Math::Geometry::Ray& r, const HitRecord& record, Color& attenuation,
            Math::Geometry::Ray& scattered) const override;

    private:
        Color m_albedo;
    };

    class Metal final : public Material
    {
    public:
        explicit Metal(const Color& a, float fuzz);

        bool Scatter(const Math::Geometry::Ray& r, const HitRecord& record, Color& attenuation,
            Math::Geometry::Ray& scattered) const override;

    private:
        Color m_albedo;
        float m_fuzz;
    };

    class Dielectric final : public Material
    {
    public:
        explicit Dielectric(float indexOfRefraction);

        bool Scatter(const Math::Geometry::Ray& r, const HitRecord& record, Color& attenuation,
            Math::Geometry::Ray& scattered) const override;

    private:
        static float Reflectance(float cosine, float refractionIndex)
        {
            // 菲涅耳反射的Schlick近似
            float r0 = (1.0f - refractionIndex) / (1.0f + refractionIndex);
            r0 = r0 * r0;
            return r0 + (1.0f - r0) * std::pow(1.0f - cosine, 5.0f);
        }

        float m_indexOfRefraction;
    };
}
