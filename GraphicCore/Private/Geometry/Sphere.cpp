/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Geometry/Sphere.h"
#include <numbers>

namespace TG::Geometry
{
    Sphere::Sphere() = default;

    Sphere::~Sphere() = default;

    Mesh Sphere::ToMesh() const
    {
        Mesh mesh;

        const auto theta = static_cast<float>(std::numbers::pi / (m_rings + 1));
        const auto phi = static_cast<float>(std::numbers::pi * 2 / (m_segments + 1));

        mesh.vertices.push_back({ 0, m_radius, 0 });
        for (unsigned int i = 1; i <= m_rings; ++i)
        {
            for (unsigned int j = 0; j < m_segments; ++j)
            {
                mesh.vertices.push_back({
                    m_radius * std::cos(phi * static_cast<float>(j)),
                    m_radius * std::cos(theta * static_cast<float>(i)),
                    m_radius * std::sin(phi * static_cast<float>(j)) });
            }
        }
        mesh.vertices.push_back({ 0, -m_radius, 0 });

        for (unsigned int i = 1; i <= m_segments; ++i)
        {
            mesh.indices.push_back(0);
            mesh.indices.push_back(i % m_segments + 1);
            mesh.indices.push_back(i);
        }
        for (unsigned int i = 0; i < mesh.vertices.size(); ++i)
        {
            for (unsigned int j = 0; j < m_segments; ++j)
            {

            }
        }
        for (unsigned int i = 1; i <= m_segments; ++i)
        {
            std::uint32_t southPoleIndex = mesh.vertices.size() - 1;
            mesh.indices.push_back(southPoleIndex);
            mesh.indices.push_back(southPoleIndex - i);
            mesh.indices.push_back(southPoleIndex - i % m_segments - 1);
        }

        for (unsigned int i = 0; i < mesh.vertices.size(); ++i)
            mesh.colors.push_back({ 1.0f, 1.0f, 1.0f });

        mesh.uvs.assign({
            { 0.0f, 0.0f },
            { 1.0f, 0.0f },
            { 1.0f, 1.0f },
            { 0.0f, 1.0f },
        });
        mesh.normals.assign({
            { 0.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 1.0f },
        });
        mesh.tangents.assign({
            { 1.0f, 0.0f, 0.0f },
            { 1.0f, 0.0f, 0.0f },
            { 1.0f, 0.0f, 0.0f },
            { 1.0f, 0.0f, 0.0f },
        });
        return mesh;
    }
}
