/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Core.hpp"
#include <vector>

namespace TG::Rendering
{
    class Mesh
    {
    public:
        std::vector<Math::Vector3f> vertices;
        std::vector<std::uint32_t>  indices;
        std::vector<Math::Vector2f> uvs;
        std::vector<Math::Vector3f> normals;
        std::vector<Math::Vector3f> tangents;
        std::vector<Math::Vector3f> colors;
    };
}
