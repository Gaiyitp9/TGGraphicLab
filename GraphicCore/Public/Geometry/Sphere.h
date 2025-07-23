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

    class Sphere
    {
    public:
        Sphere();
        ~Sphere();

        [[nodiscard]] Mesh ToMesh() const;

    private:
        SphereType m_type{ SphereType::UV };

        float m_radius{ 1.0f };
        unsigned int m_segments{ 10 };
        unsigned int m_rings{ 10 };
    };
}

