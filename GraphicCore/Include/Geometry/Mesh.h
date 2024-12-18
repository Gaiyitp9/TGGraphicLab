/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Core.hpp"
#include <vector>

namespace TG::Geometry
{
    class Mesh
    {
    public:
        Mesh();

    private:
        std::vector<Math::Vector3F> m_vertices;
        std::vector<std::uint32_t> m_indices;
        std::vector<Math::Vector3F> m_normals;
        std::vector<Math::Vector3F> m_tangents;
        std::vector<Math::Vector2F> m_uvs;
    };
}
