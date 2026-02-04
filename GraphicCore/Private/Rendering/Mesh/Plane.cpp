/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Rendering/Mesh/Plane.h"

namespace TG::Rendering
{
    Plane::Plane()
    {
        Regenerate();
    }

    Plane::Plane(float width, float height, int rows, int columns)
        : m_width(width), m_height(height), m_rows(rows), m_columns(columns)
    {
        Regenerate();
    }

    Plane::~Plane() = default;

    void Plane::Regenerate()
    {
        const float deltaX = m_width / m_columns;
        const float deltaZ = m_height / m_rows;
        const float deltaU = 1.0f / m_columns;
        const float deltaV = 1.0f / m_rows;
        const Math::Vector3f origin{ -m_width / 2, 0, -m_height / 2 };

        for (int i = 0; i <= m_rows; ++i)
        {
            for (int j = 0; j <= m_columns; ++j)
            {
                vertices.emplace_back(origin + Math::Vector3f(j * deltaX, 0, i * deltaZ));
                uvs.emplace_back(j * deltaU, i * deltaV);
                normals.emplace_back(0.0f, 1.0f, 0.0f);
                tangents.emplace_back(1.0f, 0.0f, 0.0f);
                colors.emplace_back(1.0f, 1.0f, 1.0f);
            }
        }

        for (int i = 0; i < m_rows; ++i)
        {
            for (int j = 0; j < m_columns; ++j)
            {
                const int index0 = i * (m_columns + 1) + j;
                const int index1 = index0 + 1;
                const int index2 = (i + 1) * (m_columns + 1) + j;
                const int index3 = index2 + 1;
                indices.push_back(index0);
                indices.push_back(index2);
                indices.push_back(index1);
                indices.push_back(index1);
                indices.push_back(index2);
                indices.push_back(index3);
            }
        }
    }
}
