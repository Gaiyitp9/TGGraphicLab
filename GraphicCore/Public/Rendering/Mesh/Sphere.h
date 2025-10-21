/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Mesh.h"

namespace TG::Rendering
{
    enum class SphereType
    {
        UV,
        Icosahedron,
    };

    class Sphere : public Mesh
    {
    public:
        Sphere();
        Sphere(float radius, int columns, int rows);
        ~Sphere();

        void Regenerate();

    private:
        static Math::Vector3f CalculateTangent(const Math::Vector3f& v1, const Math::Vector2f& uv1,
            const Math::Vector3f& v2, const Math::Vector2f& uv2,
            const Math::Vector3f& v3, const Math::Vector2f& uv3);

        SphereType m_type{ SphereType::UV };

        float m_radius{ 1.0f };
        int m_columns{ 20 };
        int m_rows{ 20 };
    };
}

