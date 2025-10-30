/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/Raytrace/Interval.h"
#include "Math/Core.hpp"
#include "Math/Geometry/Ray.h"

namespace TG::Rendering
{
    struct HitRecord
    {
        Math::Vector3f  position;
        Math::Vector3f  normal;
        Math::Vector3f  tangent;
        Math::Vector3f  binormal;
        float           t = 0;
        bool            frontFace = true;

        void SetFaceNormal(const Math::Geometry::Ray& r, const Math::Vector3f& outwardNormal)
        {
            frontFace = r.Direction().Dot(outwardNormal) < 0;
            normal = frontFace ? outwardNormal : -outwardNormal;
            // float d = normal.Dot(position);
            // if (std::abs(normal.X()) > Epsilon)
            // {
            //     tangent.SetY(p.Y() - 1.0);
            //     tangent.SetZ(p.Z() - 1.0);
            //     tangent.SetX((d - normal.Y() * tangent.Y() - normal.Z() * tangent.Z()) / normal.X());
            // }
            // else if (std::abs(normal.Y()) > Epsilon)
            // {
            //     tangent.SetX(p.X() - 1.0);
            //     tangent.SetZ(p.Z() - 1.0);
            //     tangent.SetY((d - normal.X() * tangent.X() - normal.Z() * tangent.Z()) / normal.Y());
            // }
            // else
            // {
            //     tangent.SetX(p.X() - 1.0);
            //     tangent.SetY(p.Y() - 1.0);
            //     tangent.SetZ((d - normal.X() * tangent.X() - normal.Y() * tangent.Y()) / normal.Z());
            // }
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
