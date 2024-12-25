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
        Mesh() = default;

        std::vector<Math::Vector3F> vertices;
        std::vector<std::uint32_t>  indices;
        std::vector<Math::Vector2F> uv;
        std::vector<Math::Vector3F> normals;
        std::vector<Math::Vector3F> tangents;
    };
}
