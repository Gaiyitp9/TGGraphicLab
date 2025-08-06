/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Rendering/OpenGLExample/SkyBox.h"
#include "Geometry/Primitives.h"

namespace TG::Rendering
{
    SkyBox::SkyBox() : m_vertexShader("Assets/Shaders/GLSL/SkyBox.vert", ShaderStage::Vertex),
        m_fragmentShader("Assets/Shaders/GLSL/SkyBox.frag", ShaderStage::Fragment)
    {
        m_cubeMesh = Geometry::CreatePrimitive(Geometry::PrimitiveType::Cube);

        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);
        glGenVertexArrays(1, &m_VAO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        const auto positionByteSize = static_cast<GLsizeiptr>(3 * sizeof(float) * m_cubeMesh.vertices.size());
        glBufferData(GL_ARRAY_BUFFER, positionByteSize, m_cubeMesh.vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(sizeof(std::uint32_t) * m_cubeMesh.indices.size()),
            m_cubeMesh.indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);

        m_cubeMap.Upload("Assets/Textures/CubeMaps/Fjaderholmarna");
        m_fragmentShader.SetInt("skybox", 0);

        glGenProgramPipelines(1, &m_pipeline);
        glBindProgramPipeline(m_pipeline);
        glUseProgramStages(m_pipeline, GL_VERTEX_SHADER_BIT, CastID<GLuint>(m_vertexShader.GetID()));
        glUseProgramStages(m_pipeline, GL_FRAGMENT_SHADER_BIT, CastID<GLuint>(m_fragmentShader.GetID()));
    }

    SkyBox::~SkyBox()
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
        glDeleteProgramPipelines(1, &m_pipeline);
    }

    void SkyBox::Render(const Camera& camera) const
    {
        const bool prevDepthTest = glIsEnabled(GL_DEPTH_TEST);
        GLint prevDepthFunc;
        if (!prevDepthTest)
            glEnable(GL_DEPTH_TEST);
        else
        {
            glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFunc);
            glDepthFunc(GL_LEQUAL);
        }

        glActiveTexture(GL_TEXTURE_CUBE_MAP);
        glBindTexture(GL_TEXTURE_CUBE_MAP, CastID<GLuint>(m_cubeMap.GetID()));

        glBindVertexArray(m_VAO);
        glBindProgramPipeline(m_pipeline);

        Math::Matrix4f view = camera.ViewMatrix();
        view.Block<3, 1>(0, 3) = Math::Vector3f::Zero();
        m_vertexShader.SetMat4("view", view);
        m_vertexShader.SetMat4("projection", camera.ProjectionMatrix());

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_cubeMesh.indices.size()), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        if (!prevDepthTest)
            glDisable(GL_DEPTH_TEST);
        else
            glDepthFunc(prevDepthFunc);
    }
}
