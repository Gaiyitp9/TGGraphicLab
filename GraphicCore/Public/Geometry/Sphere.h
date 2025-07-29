/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Mesh.h"

namespace TG::Geometry
{
    enum class SphereType
    {
        UV,
        Geodesic,
    };

    class Sphere : public Mesh
    {
    public:
        Sphere();
        ~Sphere();

        void Update();

    private:
        static Math::Vector3F CalculateTangent(const Math::Vector3F& v1, const Math::Vector2F& uv1,
            const Math::Vector3F& v2, const Math::Vector2F& uv2,
            const Math::Vector3F& v3, const Math::Vector2F& uv3);

        SphereType m_type{ SphereType::UV };

        float m_radius{ 1.0f };
        int m_columns{ 10 };
        int m_rows{ 10 };
    };
}

