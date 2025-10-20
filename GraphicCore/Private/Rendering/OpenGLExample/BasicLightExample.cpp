/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Rendering/OpenGLExample/BasicLightExample.h"
#include "Rendering/Color/StandardColors.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "Rendering/RayTracing/PathTracer.h"

namespace TG::Rendering
{
    struct CameraData
    {
        Math::Matrix4f view;
        Math::Matrix4f projection;
    };

    struct RenderData
    {
        Math::Vector4f lightDirection;
        Math::Vector4f lightColor;
        Math::Vector4f viewPosition;
    };

    BasicLightExample::BasicLightExample(const std::weak_ptr<IDefaultVideoPort> &videoPort,
        const std::weak_ptr<ITimer> &timer) : m_camera(videoPort, timer),
        m_vertexShader("Assets/Shaders/GLSL/LightingModel/Phong/BasicLighting.vert", ShaderStage::Vertex),
        m_fragmentShader("Assets/Shaders/GLSL/LightingModel/Phong/Phong.frag", ShaderStage::Fragment),
        m_wireframeGeometryShader("Assets/Shaders/GLSL/Wireframe.geom", ShaderStage::Geometry),
        m_wireframeFragmentShader("Assets/Shaders/GLSL/Wireframe.frag", ShaderStage::Fragment)
    {
        m_sphereProperties[0] = {
            Math::Vector3f{ -6.0f, 1.0f, -10.0f },
            SaddleBrown,
            0.3f, 0.5f, 8.0f
        };
        m_sphereProperties[1] = {
            Math::Vector3f{ -3.0f, 1.0f, -10.0f },
            SaddleBrown,
            0.3f, 0.5f, 16.0f
        };
        m_sphereProperties[2] = {
            Math::Vector3f{ 0.0f, 1.0f, -10.0f },
            SaddleBrown,
            0.3f, 0.5f, 32.0f
        };
        m_sphereProperties[3] = {
            Math::Vector3f{ 3.0f, 1.0f, -10.0f },
            SaddleBrown,
            0.3f, 0.5f, 64.0f
        };
        m_sphereProperties[4] = {
            Math::Vector3f{ 6.0f, 1.0f, -10.0f },
            SaddleBrown,
            0.3f, 0.5f, 128.0f
        };

        m_lightDirection = Math::Vector4f{ 1.0f, 5.0f, 1.0f, 0.0f };
        m_lightColor = White;

        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);
        glGenVertexArrays(1, &m_VAO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        const auto verticesByteSize = static_cast<GLsizeiptr>(3 * sizeof(float) * m_sphereMesh.vertices.size());
        const auto uvByteSize = static_cast<GLsizeiptr>(2 * sizeof(float) * m_sphereMesh.uvs.size());
        const auto normalByteSize = static_cast<GLsizeiptr>(3 * sizeof(float) * m_sphereMesh.normals.size());
        glBufferData(GL_ARRAY_BUFFER, verticesByteSize + uvByteSize + normalByteSize, nullptr, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, verticesByteSize, m_sphereMesh.vertices.data());
        glBufferSubData(GL_ARRAY_BUFFER, verticesByteSize, uvByteSize, m_sphereMesh.uvs.data());
        glBufferSubData(GL_ARRAY_BUFFER, verticesByteSize + uvByteSize, normalByteSize, m_sphereMesh.normals.data());

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
            reinterpret_cast<void*>(verticesByteSize));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
            reinterpret_cast<void*>(verticesByteSize + uvByteSize));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(sizeof(std::uint32_t) * m_sphereMesh.indices.size()),
            m_sphereMesh.indices.data(), GL_STATIC_DRAW);
        glBindVertexArray(0);

        glGenBuffers(1, &m_cameraUbo);
        glGenBuffers(1, &m_renderUbo);
        glBindBuffer(GL_UNIFORM_BUFFER, m_cameraUbo);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraData), nullptr, GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_cameraUbo);
        glBindBuffer(GL_UNIFORM_BUFFER, m_renderUbo);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(RenderData), nullptr, GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_renderUbo);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glGenProgramPipelines(1, &m_pipeline);
        glBindProgramPipeline(m_pipeline);
        glUseProgramStages(m_pipeline, GL_VERTEX_SHADER_BIT, CastID<GLuint>(m_vertexShader.GetID()));
        glUseProgramStages(m_pipeline, GL_FRAGMENT_SHADER_BIT, CastID<GLuint>(m_fragmentShader.GetID()));

        glEnable(GL_DEPTH_TEST);

        // m_albedoTexture.Upload("Assets/Textures/wall.jpg");
        m_albedoTexture.Upload(m_defaultAlbedo, 1, 1, TextureFormat::RGBA);
    }

    BasicLightExample::~BasicLightExample()
    {
        glDeleteProgramPipelines(1, &m_pipeline);
        glDeleteBuffers(1, &m_renderUbo);
        glDeleteBuffers(1, &m_cameraUbo);
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);

        if (m_renderThread)
            m_renderThread->join();
    }

    void BasicLightExample::Render()
    {
        m_camera.Update();

        Color clearColor = DimGray;
        glClearColor(clearColor.R(), clearColor.G(), clearColor.B(), clearColor.A());
        glClearDepth(1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (m_wireframe)
        {
            glUseProgramStages(m_pipeline, GL_GEOMETRY_SHADER_BIT, CastID<GLuint>(m_wireframeGeometryShader.GetID()));
            glUseProgramStages(m_pipeline, GL_FRAGMENT_SHADER_BIT, CastID<GLuint>(m_wireframeFragmentShader.GetID()));
        }
        else
        {
            glUseProgramStages(m_pipeline, GL_GEOMETRY_SHADER_BIT, 0);
            glUseProgramStages(m_pipeline, GL_FRAGMENT_SHADER_BIT, CastID<GLuint>(m_fragmentShader.GetID()));
        }

        glBindBuffer(GL_UNIFORM_BUFFER, m_cameraUbo);
        CameraData cameraData;
        cameraData.view = m_camera.ViewMatrix();
        cameraData.projection = m_camera.ProjectionMatrix();
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CameraData), &cameraData);

        glBindBuffer(GL_UNIFORM_BUFFER, m_renderUbo);
        RenderData renderData;
        renderData.lightDirection = m_lightDirection;
        renderData.lightColor = m_lightColor.ToVector4();
        renderData.viewPosition.Block<3, 1>(0, 0) = m_camera.position;
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(RenderData), &renderData);

        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, CastID<GLuint>(m_albedoTexture.GetID()));

        glBindProgramPipeline(m_pipeline);
        glBindVertexArray(m_VAO);
        for (const auto& [position, color, ambientStrength, specularStrength, shininess] : m_sphereProperties)
        {
            Math::Transform<float, 3> modelTransform;
            modelTransform.Translate(position);
            m_vertexShader.SetMat4("model", modelTransform.ToTransformMatrix());

            m_fragmentShader.SetFloat3("objectColor", color.R(), color.G(), color.B());
            m_fragmentShader.SetFloat("ambientStrength", ambientStrength);
            m_fragmentShader.SetFloat("specularStrength", specularStrength);
            m_fragmentShader.SetFloat("shininess", shininess);

            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_sphereMesh.indices.size()),
                GL_UNSIGNED_INT, nullptr);
        }
        glBindVertexArray(0);

        m_viewportGrid.Render(m_camera);
        m_viewportCompass.Render(m_camera);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        const ImGuiIO& io = ImGui::GetIO();
        ImGui::Begin("General");
        ImGui::Checkbox("Draw wireframe", &m_wireframe);
        ImGui::BeginDisabled(!m_renderDone);
        if (ImGui::Button("Render"))
        {
            if (m_renderThread)
            {
                m_renderThread->join();
                m_renderThread.reset();
            }
            m_renderThread = std::make_unique<std::thread>([this] {
                PathTraceData pathTraceData;
                pathTraceData.aspectRatio = m_camera.AspectRatio();
                pathTraceData.focalLength = m_camera.nearPlane;
                pathTraceData.fov = m_camera.fov;
                pathTraceData.cameraPosition = m_camera.position;
                pathTraceData.front = m_camera.front;
                pathTraceData.right = m_camera.right;
                pathTraceData.up = m_camera.up;
                RunPathTracer(m_renderProcess, m_renderDone, pathTraceData);
            });
        }
        ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::ProgressBar(m_renderProcess, ImVec2(200.0f, 0.0f));
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}
