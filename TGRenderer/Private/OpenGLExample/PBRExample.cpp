/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "PBRExample.h"
#include "Rendering/OpenGL/OpenGLRenderer.h"
#include "Rendering/Color/StandardColors.h"
#include "Math/Transform/Transform.hpp"

namespace TG
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

    PBRExample::PBRExample(const Rendering::Renderer* renderer, const ITimer& timer)
        : m_camera{renderer->GetContext().VideoPort(), timer},
        m_lightDirection{1.0f, 1.0f, 1.0f, 0.0f},
        m_lightColor{Rendering::White},
        m_vertexShader{"Assets/Shaders/GLSL/OpenGL/LightingModel/PBR/pbr.vert", Rendering::ShaderStage::Vertex},
        m_fragmentShader{"Assets/Shaders/GLSL/OpenGL/LightingModel/PBR/pbr.frag", Rendering::ShaderStage::Fragment}
    {
        auto openglRenderer{dynamic_cast<const Rendering::OpenGLRenderer*>(renderer)};
        m_camera.aspectRatio = static_cast<float>(openglRenderer->SceneWidth()) /
            static_cast<float>(openglRenderer->SceneHeight());

        glGenVertexArrays(1, &m_sphereVAO);
        glGenBuffers(1, &m_sphereVBO);
        glGenBuffers(1, &m_sphereEBO);

        glBindVertexArray(m_sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_sphereVBO);
        auto verticesByteSize{static_cast<GLsizeiptr>(3 * sizeof(float) * m_sphereMesh.vertices.size())};
        auto uvByteSize{static_cast<GLsizeiptr>(2 * sizeof(float) * m_sphereMesh.uvs.size())};
        auto normalByteSize{static_cast<GLsizeiptr>(3 * sizeof(float) * m_sphereMesh.normals.size())};
        auto tangentByteSize{static_cast<GLsizeiptr>(3 * sizeof(float) * m_sphereMesh.tangents.size())};
        glBufferData(GL_ARRAY_BUFFER, verticesByteSize + uvByteSize + normalByteSize + tangentByteSize,
            nullptr, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, verticesByteSize, m_sphereMesh.vertices.data());
        glBufferSubData(GL_ARRAY_BUFFER, verticesByteSize, uvByteSize, m_sphereMesh.uvs.data());
        glBufferSubData(GL_ARRAY_BUFFER, verticesByteSize + uvByteSize, normalByteSize,
            m_sphereMesh.normals.data());
        glBufferSubData(GL_ARRAY_BUFFER, verticesByteSize + uvByteSize + normalByteSize, tangentByteSize,
            m_sphereMesh.tangents.data());

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
            reinterpret_cast<void*>(verticesByteSize));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
            reinterpret_cast<void*>(verticesByteSize + uvByteSize));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
            reinterpret_cast<void*>(verticesByteSize + uvByteSize + normalByteSize));
        glEnableVertexAttribArray(3);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_sphereEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(sizeof(std::uint32_t) * m_sphereMesh.indices.size()),
            m_sphereMesh.indices.data(), GL_STATIC_DRAW);
        glBindVertexArray(0);

        glGenBuffers(1, &m_cameraUbo);
        glBindBuffer(GL_UNIFORM_BUFFER, m_cameraUbo);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraData), nullptr, GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_cameraUbo);
        glGenBuffers(1, &m_renderUbo);
        glBindBuffer(GL_UNIFORM_BUFFER, m_renderUbo);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(RenderData), nullptr, GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_renderUbo);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glGenProgramPipelines(1, &m_pipeline);
        glBindProgramPipeline(m_pipeline);
        glUseProgramStages(m_pipeline, GL_VERTEX_SHADER_BIT,
            Rendering::CastID<Rendering::OpenGLID>(m_vertexShader.GetID()));
        glUseProgramStages(m_pipeline, GL_FRAGMENT_SHADER_BIT,
            Rendering::CastID<Rendering::OpenGLID>(m_fragmentShader.GetID()));

        glEnable(GL_DEPTH_TEST);

        m_albedo.Upload("Assets/Textures/RustedIron/rusted_iron_baseColor.png");
        m_normalMap.Upload("Assets/Textures/RustedIron/rusted_iron_normal.png");
        m_metallicRoughness.Upload("Assets/Textures/RustedIron/rusted_iron_metallic_roughness.png");
    }

    PBRExample::~PBRExample()
    {
        glDeleteProgramPipelines(1, &m_pipeline);
        glDeleteBuffers(1, &m_cameraUbo);
        glDeleteBuffers(1, &m_renderUbo);
        glDeleteVertexArrays(1,&m_sphereVAO);
        glDeleteBuffers(1,&m_sphereEBO);
        glDeleteBuffers(1,&m_sphereVBO);
    }

    void PBRExample::Draw()
    {
        m_camera.Update();

        // 开启面剔除，剔除背面
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        Rendering::Color clearColor = clearColor.SRGBToLinear(Rendering::DimGray);
        glClearColor(clearColor.R(), clearColor.G(), clearColor.B(), clearColor.A());
        glClearDepthf(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
        glBindTexture(GL_TEXTURE_2D, Rendering::CastID<Rendering::OpenGLID>(m_albedo.GetID()));
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, Rendering::CastID<Rendering::OpenGLID>(m_normalMap.GetID()));
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, Rendering::CastID<Rendering::OpenGLID>(m_metallicRoughness.GetID()));

        glBindProgramPipeline(m_pipeline);
        glBindVertexArray(m_sphereVAO);

        // 绘制球
        Math::Transform<float, 3> modelTransform;
        Math::Vector3f movingPosition = m_sphereProperties.position;
        modelTransform.Translate(movingPosition);
        m_vertexShader.SetMat4("model", modelTransform.ToTransformMatrix());

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_sphereMesh.indices.size()),
            GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);

        m_skybox.Render(m_camera);
        m_viewportCompass.Render(m_camera);
    }

    void PBRExample::DrawUI()
    {

    }

    void PBRExample::OnViewportChanged(unsigned width, unsigned height)
    {
        m_camera.aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    }
}
