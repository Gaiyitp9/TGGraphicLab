/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "ViewportGridGL.h"

namespace TG
{
    ViewportGridGL::ViewportGridGL() : m_vertexShader("Shaders/GLSL/ViewportGrid.vert", ShaderStageGL::Vertex),
        m_fragmentShader("Shaders/GLSL/ViewportGrid.frag", ShaderStageGL::Fragment),
        m_geometryShader("Shaders/GLSL/ViewportGrid.geom", ShaderStageGL::Geometry)
    {
        glGenVertexArrays(1, &m_VAO);
        glGenProgramPipelines(1, &m_pipeline);
        glUseProgramStages(m_pipeline, GL_VERTEX_SHADER_BIT, m_vertexShader.GetID());
        glUseProgramStages(m_pipeline, GL_FRAGMENT_SHADER_BIT, m_fragmentShader.GetID());
        glUseProgramStages(m_pipeline, GL_GEOMETRY_SHADER_BIT, m_geometryShader.GetID());

        glLineWidth(1.5);
    }

    ViewportGridGL::~ViewportGridGL()
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteProgramPipelines(1, &m_pipeline);
    }

    void ViewportGridGL::Render(const Camera& camera) const
    {
        Frustum frustum = camera.ViewFrustum();

        // 计算xz平面的aabb
        float minX = frustum.corners[0].X();
        float maxX = frustum.corners[0].X();
        float minZ = frustum.corners[0].Z();
        float maxZ = frustum.corners[0].Z();
        for (int i = 1; i < 8; ++i)
        {
            minX = std::min(minX, frustum.corners[i].X());
            maxX = std::max(maxX, frustum.corners[i].X());
            minZ = std::min(minZ, frustum.corners[i].Z());
            maxZ = std::max(maxZ, frustum.corners[i].Z());
        }
        minX = std::floor(minX);
        maxX = std::ceil(maxX);
        minZ = std::floor(minZ);
        maxZ = std::ceil(maxZ);
        float size = maxX - minX;
        size = std::max(size, maxZ - minZ);

        constexpr int drawSize = 63;
        const int drawCount = std::ceil(size / drawSize);

        m_geometryShader.SetMat4("view", camera.ViewMatrix());
        m_geometryShader.SetMat4("projection", camera.ProjectionMatrix());
        m_fragmentShader.SetFloat3("gridColor", 0.6f, 0.6f, 0.6f);

        glBindVertexArray(m_VAO);
        glBindProgramPipeline(m_pipeline);
        glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, "ViewportGrid");
        for (int i = 0; i < drawCount; ++i)
        {
            for (int j = 0; j < drawCount; ++j)
            {
                m_geometryShader.SetInt2("origin", static_cast<int>(minX) + i * drawSize,
                    static_cast<int>(minZ) + j * drawSize);
                glDrawArrays(GL_POINTS, 0, 1);
            }
        }
        glPopDebugGroup();
        glBindVertexArray(0);
    }
}
