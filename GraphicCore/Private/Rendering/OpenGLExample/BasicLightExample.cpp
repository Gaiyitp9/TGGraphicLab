/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Rendering/OpenGLExample/BasicLightExample.h"
#include "Rendering/Color/StandardColors.h"
#include "Rendering/RayTracing/PathTracer.h"
#include "Math/Transform/Transform.hpp"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"

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
        const std::weak_ptr<ITimer> &timer) : m_timer(timer), m_camera(videoPort, timer),
        m_planeMesh(20.0f, 20.0f, 20, 20),
        m_planeProperty{
            Math::Vector3f{ 0.0f, 0.01f, -10.0f },
            SlateGray,
            0.3f, 0.5f, 64.0f },
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

        m_lightDirection = Math::Vector4f{ 1.0f, 1.0f, 1.0f, 0.0f };
        m_lightColor = White;

        glGenBuffers(1, &m_sphereVBO);
        glGenBuffers(1, &m_sphereEBO);
        glGenVertexArrays(1, &m_sphereVAO);

        glBindVertexArray(m_sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_sphereVBO);
        auto verticesByteSize = static_cast<GLsizeiptr>(3 * sizeof(float) * m_sphereMesh.vertices.size());
        auto uvByteSize = static_cast<GLsizeiptr>(2 * sizeof(float) * m_sphereMesh.uvs.size());
        auto normalByteSize = static_cast<GLsizeiptr>(3 * sizeof(float) * m_sphereMesh.normals.size());
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

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_sphereEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(sizeof(std::uint32_t) * m_sphereMesh.indices.size()),
            m_sphereMesh.indices.data(), GL_STATIC_DRAW);
        glBindVertexArray(0);

        glGenBuffers(1, &m_planeVBO);
        glGenBuffers(1, &m_planeEBO);
        glGenVertexArrays(1, &m_planeVAO);

        glBindVertexArray(m_planeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_planeVBO);
        verticesByteSize = static_cast<GLsizeiptr>(3 * sizeof(float) * m_planeMesh.vertices.size());
        uvByteSize = static_cast<GLsizeiptr>(2 * sizeof(float) * m_planeMesh.uvs.size());
        normalByteSize = static_cast<GLsizeiptr>(3 * sizeof(float) * m_planeMesh.normals.size());
        glBufferData(GL_ARRAY_BUFFER, verticesByteSize + uvByteSize + normalByteSize, nullptr, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, verticesByteSize, m_planeMesh.vertices.data());
        glBufferSubData(GL_ARRAY_BUFFER, verticesByteSize, uvByteSize, m_planeMesh.uvs.data());
        glBufferSubData(GL_ARRAY_BUFFER, verticesByteSize + uvByteSize, normalByteSize, m_planeMesh.normals.data());

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
            reinterpret_cast<void*>(verticesByteSize));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
            reinterpret_cast<void*>(verticesByteSize + uvByteSize));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_planeEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(sizeof(std::uint32_t) * m_planeMesh.indices.size()),
            m_planeMesh.indices.data(), GL_STATIC_DRAW);
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
        glDeleteVertexArrays(1, &m_planeVAO);
        glDeleteBuffers(1, &m_planeVBO);
        glDeleteBuffers(1, &m_planeEBO);
        glDeleteVertexArrays(1, &m_sphereVAO);
        glDeleteBuffers(1, &m_sphereVBO);
        glDeleteBuffers(1, &m_sphereEBO);

        if (m_renderThread)
            m_renderThread->join();
    }

    void BasicLightExample::Render()
    {
        m_camera.Update();

        // 开启面剔除，剔除背面
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        // 正面朝向设置为顺时针
        // glFrontFace(GL_CW);

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
        glBindVertexArray(m_sphereVAO);
        for (const auto& [position, color, ambientStrength, specularStrength, shininess] : m_sphereProperties)
        {
            Math::Transform<float, 3> modelTransform;
            Math::Vector3f movingPosition = position;
            // movingPosition.X() += 3.0f * std::sin(m_timer.lock()->TotalTime() * 0.002f);
            modelTransform.Translate(movingPosition);
            m_vertexShader.SetMat4("model", modelTransform.ToTransformMatrix());

            m_fragmentShader.SetFloat3("objectColor", color.R(), color.G(), color.B());
            m_fragmentShader.SetFloat("ambientStrength", ambientStrength);
            m_fragmentShader.SetFloat("specularStrength", specularStrength);
            m_fragmentShader.SetFloat("shininess", shininess);

            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_sphereMesh.indices.size()),
                GL_UNSIGNED_INT, nullptr);
        }

        glBindVertexArray(m_planeVAO);
        Math::Transform<float, 3> planeModelTransform;
        planeModelTransform.Translate(m_planeProperty.position);
        m_vertexShader.SetMat4("model", planeModelTransform.ToTransformMatrix());

        m_fragmentShader.SetFloat3("objectColor",
            m_planeProperty.color.R(), m_planeProperty.color.G(), m_planeProperty.color.B());
        m_fragmentShader.SetFloat("ambientStrength", m_planeProperty.ambientStrength);
        m_fragmentShader.SetFloat("specularStrength", m_planeProperty.specularStrength);
        m_fragmentShader.SetFloat("shininess", m_planeProperty.shininess);

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_planeMesh.indices.size()),
            GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        m_viewportGrid.Render(m_camera);
        m_viewportCompass.Render(m_camera);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        const ImGuiIO& io = ImGui::GetIO();
        ImGui::Begin("General");
        ImGui::Checkbox("Draw wireframe", &m_wireframe);
        ImGui::BeginDisabled(!m_pathTracer.IsReady());
        if (ImGui::Button("Render"))
        {
            if (m_renderThread)
            {
                m_renderThread->join();
                m_renderThread.reset();
            }
            m_renderThread = std::make_unique<std::thread>([this] {
                RayTracing::PathTraceData pathTraceData;
                pathTraceData.imageWidth = 1600;
                pathTraceData.imageHeight = 900;
                pathTraceData.cameraPosition = m_camera.position;
                pathTraceData.front = m_camera.front;
                pathTraceData.up = m_camera.up;
                pathTraceData.right = m_camera.right;
                pathTraceData.aspectRatio = m_camera.AspectRatio();
                pathTraceData.focalDistance = 8.0f;
                pathTraceData.fov = m_camera.fov;
                pathTraceData.defocusAngle = 10.0f;
                pathTraceData.samplesPerPixel = 50;
                pathTraceData.maxDepth = 50;
                m_pathTracer.Run(pathTraceData);
            });
        }
        ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::ProgressBar(m_pathTracer.Process(), ImVec2(200.0f, 0.0f));
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}
