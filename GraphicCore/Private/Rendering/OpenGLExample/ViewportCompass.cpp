/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "ViewportCompass.h"
#include "Rendering/ViewProjectionMatrix.hpp"

namespace TG::Rendering
{
    ViewportCompass::ViewportCompass()
        : m_vertexShader("Shaders/GLSL/ViewportCompass.vert", ShaderStage::Vertex),
        m_fragmentShader("Shaders/GLSL/ViewportCompass.frag", ShaderStage::Fragment),
        m_axisVertices{
            // 位置           颜色
            0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        }
    {
        glGenBuffers(1, &m_VBO);
        glGenVertexArrays(1, &m_VAO);
        glGenProgramPipelines(1, &m_pipeline);

        glUseProgramStages(m_pipeline, GL_VERTEX_SHADER_BIT, CastID<GLuint>(m_vertexShader.GetID()));
        glUseProgramStages(m_pipeline, GL_FRAGMENT_SHADER_BIT, CastID<GLuint>(m_fragmentShader.GetID()));

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_axisVertices.size() * sizeof(float)),
            m_axisVertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
            reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);
    }

    ViewportCompass::~ViewportCompass()
    {
        glDeleteProgramPipelines(1, &m_pipeline);
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
    }

    void ViewportCompass::Render(const Camera &camera) const
    {
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glViewport(0, 0, 100, 100);

        glBindVertexArray(m_VAO);
        glBindProgramPipeline(m_pipeline);

        Math::Matrix4F view = camera.ViewMatrix();
        view.Block<3, 1>(0, 3) = Math::Vector3F::Zero();
        m_vertexShader.SetMat4("view", view);
        m_vertexShader.SetMat4("projection", Orthographic(-1.2f, 1.2f, -1.2f, 1.2f, -2.0f, 2.0f));

        glDrawArrays(GL_LINES, 0, 6);
        glBindVertexArray(0);

        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
    }
}
