/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/RayTracing/Interval.h"
#include "Math/Geometry/Ray.h"
#include <memory>

namespace TG::Rendering::RayTracing
{
    class Material;

    struct HitRecord
    {
        Math::Vector3f  position;
        Math::Vector3f  normal;
        Math::Vector3f  tangent;
        Math::Vector3f  binormal;
        std::shared_ptr<Material> material;
        float           t = 0;
        bool            frontFace = true;

        void SetFaceNormal(const Math::Geometry::Ray& r, const Math::Vector3f& outwardNormal)
        {
            frontFace = r.Direction().Dot(outwardNormal) < 0;
            normal = frontFace ? outwardNormal : -outwardNormal;
            float d = normal.Dot(position);
            if (std::abs(normal.X()) > Math::EpsilonF)
            {
                tangent.Y() = position.Y() - 1.0f;
                tangent.Z() = position.Z() - 1.0f;
                tangent.X() = (d - normal.Y() * tangent.Y() - normal.Z() * tangent.Z()) / normal.X();
            }
            else if (std::abs(normal.Y()) > Math::EpsilonF)
            {
                tangent.X() = position.X() - 1.0f;
                tangent.Z() = position.Z() - 1.0f;
                tangent.Y() = (d - normal.X() * tangent.X() - normal.Z() * tangent.Z()) / normal.Y();
            }
            else
            {
                tangent.X() = position.X() - 1.0f;
                tangent.Y() = position.Y() - 1.0f;
                tangent.Z() = (d - normal.X() * tangent.X() - normal.Y() * tangent.Y()) / normal.Z();
            }
            tangent -= position;
            tangent.Normalize();
            binormal = tangent.Cross(normal);
        }
    };

    class Hittable
    {
    public:
        Hittable() = default;
        virtual ~Hittable() = default;

        virtual bool Hit(const Math::Geometry::Ray& ray, const Interval& rayInterval, HitRecord& record) const = 0;
    };
}
