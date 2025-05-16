/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "CubeExample.h"
#include "Color/StandardColors.h"
#include "Geometry/Primitives.h"
#include "Rendering/ProjectionMatrix.hpp"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace TG
{
    CubeExample::CubeExample(const IDefaultVideoPort& videoPort, const ITimer &timer)
        : m_timer(timer), m_videoPort(videoPort),
        m_vertexShader("Shaders/GLSL/Cube.vert", ShaderStage::Vertex),
        m_fragmentShader("Shaders/GLSL/Cube.frag", ShaderStage::Fragment),
        m_geometryShader("Shaders/GLSL/Wireframe.geom", ShaderStage::Geometry)
    {
        m_cubeMesh = Geometry::CreatePrimitive(Geometry::PrimitiveType::Cube);

        m_cubePositions[0] = Math::Vector3F{ 0.0f, 0.0f, 0.0f };
        m_cubePositions[1] = Math::Vector3F{ 2.0f, 5.0f, -15.0f };
        m_cubePositions[2] = Math::Vector3F{ -1.5f, -2.2f, -2.5f };
        m_cubePositions[3] = Math::Vector3F{ -3.8f, -2.0f, -12.3f };
        m_cubePositions[4] = Math::Vector3F{ 2.4f, -0.4f, -3.5f };
        m_cubePositions[5] = Math::Vector3F{ -1.7f, 3.0f, -7.5f };
        m_cubePositions[6] = Math::Vector3F{ 1.3f, -2.0f, -2.5f };
        m_cubePositions[7] = Math::Vector3F{ 1.5f, 2.0f, -2.5f };
        m_cubePositions[8] = Math::Vector3F{ 1.5f, 0.2f, -1.5f };
        m_cubePositions[9] = Math::Vector3F{ -1.3f, 1.0f, -1.5f };

        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);
        glGenVertexArrays(1, &m_VAO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        const auto positionByteSize = static_cast<GLsizeiptr>(3 * sizeof(float) * m_cubeMesh.vertices.size());
        const auto uvByteSize = static_cast<GLsizeiptr>(2 * sizeof(float) * m_cubeMesh.uvs.size());
        glBufferData(GL_ARRAY_BUFFER, positionByteSize + uvByteSize, nullptr, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, positionByteSize, m_cubeMesh.vertices.data());
        glBufferSubData(GL_ARRAY_BUFFER, positionByteSize, uvByteSize, m_cubeMesh.uvs.data());

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), reinterpret_cast<void*>(positionByteSize));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(sizeof(std::uint32_t) * m_cubeMesh.indices.size()),
            m_cubeMesh.indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);

        m_textures[0].Upload("Resources/Textures/wall.jpg");
        m_textures[1].Upload("Resources/Textures/container.jpg");

        m_fragmentShader.SetInt("albedo0", 0);
        m_fragmentShader.SetInt("albedo1", 1);

        glGenProgramPipelines(1, &m_pipeline);
        glUseProgramStages(m_pipeline, GL_VERTEX_SHADER_BIT, m_vertexShader.GetID());
        glUseProgramStages(m_pipeline, GL_FRAGMENT_SHADER_BIT, m_fragmentShader.GetID());

        glEnable(GL_DEPTH_TEST);
    }

    CubeExample::~CubeExample()
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
        glDeleteProgramPipelines(1, &m_pipeline);
    }

    void CubeExample::Render()
    {
        Color::Color clearColor = Color::AliceBlue;

        glClearColor(clearColor.R(), clearColor.G(), clearColor.B(), clearColor.A());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (m_wireframe)
            glUseProgramStages(m_pipeline, GL_GEOMETRY_SHADER_BIT, m_geometryShader.GetID());
        else
            glUseProgramStages(m_pipeline, GL_GEOMETRY_SHADER_BIT, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textures[0].GetID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_textures[1].GetID());

        Math::Transform<float, 3> viewTransform;
        viewTransform.Translate(Math::Vector3F{ 0.0f, 0.0f, -3.0f });
        Math::Matrix4 projection = Rendering::Perspective(static_cast<float>(45.0_deg_to_rad),
            static_cast<float>(m_videoPort.Width()) / static_cast<float>(m_videoPort.Height()), 0.1f, 100.0f);
        m_vertexShader.SetMat4("view", viewTransform.ToTransformMatrix());
        m_vertexShader.SetMat4("projection", projection);

        glBindVertexArray(m_VAO);
        glBindProgramPipeline(m_pipeline);
        for (unsigned int i = 0; i < 10; ++i)
        {
            Math::Transform<float, 3> modelTransform;
            modelTransform.Translate(m_cubePositions[i]);
            const float angle = 20.0f * static_cast<float>(i);
            modelTransform.Rotate(Math::AngleAxis{ angle, Math::Vector3F{ 1.0f, 0.3f, 0.5f }});
            m_vertexShader.SetMat4("model", modelTransform.ToTransformMatrix());

    	    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_cubeMesh.indices.size()), GL_UNSIGNED_INT, nullptr);
        }
        glBindVertexArray(0);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        const ImGuiIO& io = ImGui::GetIO();
        ImGui::Begin("Settings");
        ImGui::Checkbox("Draw wireframe", &m_wireframe);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}
