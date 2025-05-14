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
    struct Vertex
    {
        Math::Vector3F position;
        Math::Vector2F uv;
        Math::Vector3F colors;
        Math::Vector3F normal;
        Math::Vector3F tangent;
    };

    class Mesh
    {
    public:
        Mesh() = default;

        [[nodiscard]] float const* Buffer() const { return m_buffer.data(); }

        std::vector<Math::Vector3F> vertices;
        std::vector<std::uint32_t>  indices;
        std::vector<Math::Vector2F> uv;
        std::vector<Math::Vector3F> colors;
        std::vector<Math::Vector3F> normals;
        std::vector<Math::Vector3F> tangents;

    private:
        std::vector<float> m_buffer;
        std::vector<std::uint32_t> m_indices;
    };
}
