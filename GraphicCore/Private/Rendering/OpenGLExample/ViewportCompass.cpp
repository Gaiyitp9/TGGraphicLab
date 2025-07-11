/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "ViewportCompass.h"
#include "Rendering/ViewProjectionMatrix.hpp"
#include "Base/Utility.h"

namespace TG::Rendering
{
    ViewportCompass::ViewportCompass()
        : m_axisVertexShader("Shaders/GLSL/ViewportCompass.vert", ShaderStage::Vertex),
          m_axisFragmentShader("Shaders/GLSL/ViewportCompass.frag", ShaderStage::Fragment),
          m_axisVertices{
              // 位置           颜色
              0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
              1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
              0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
              0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
              0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
              0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
          },
          m_axisNameVertexShader("Shaders/GLSL/CompassName.vert", ShaderStage::Vertex),
          m_axisNameFragmentShader("Shaders/GLSL/CompassName.frag", ShaderStage::Fragment),
          m_fontBuffer(LoadBinaryFile("Resources/Fonts/consola.ttf"))
    {
        glGenBuffers(1, &m_axisVBO);
        glGenVertexArrays(1, &m_axisVAO);
        glGenProgramPipelines(1, &m_axisPipeline);

        glUseProgramStages(m_axisPipeline, GL_VERTEX_SHADER_BIT, CastID<GLuint>(m_axisVertexShader.GetID()));
        glUseProgramStages(m_axisPipeline, GL_FRAGMENT_SHADER_BIT, CastID<GLuint>(m_axisFragmentShader.GetID()));

        glBindVertexArray(m_axisVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_axisVBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_axisVertices.size() * sizeof(float)),
                     m_axisVertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                              reinterpret_cast<void *>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);

        m_axisNameMesh = Geometry::CreatePrimitive(Geometry::PrimitiveType::Quad);

        glGenBuffers(1, &m_axisNameVBO);
        glGenVertexArrays(1, &m_axisNameVAO);
        glGenProgramPipelines(1, &m_axisNamePipeline);

        glUseProgramStages(m_axisNamePipeline, GL_VERTEX_SHADER_BIT, CastID<GLuint>(m_axisNameVertexShader.GetID()));
        glUseProgramStages(m_axisNamePipeline, GL_FRAGMENT_SHADER_BIT, CastID<GLuint>(m_axisNameFragmentShader.GetID()));

        glBindVertexArray(m_axisNameVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_axisNameVBO);
        const auto positionByteSize = static_cast<GLsizeiptr>(3 * sizeof(float) * m_axisNameMesh.vertices.size());
        const auto uvByteSize = static_cast<GLsizeiptr>(2 * sizeof(float) * m_axisNameMesh.vertices.size());
        glBufferData(GL_ARRAY_BUFFER, positionByteSize + uvByteSize, nullptr, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, positionByteSize, m_axisNameMesh.vertices.data());
        glBufferSubData(GL_ARRAY_BUFFER, positionByteSize, uvByteSize, m_axisNameMesh.uvs.data());
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), reinterpret_cast<void *>(positionByteSize));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);

        // 初始化字体
        if (!stbtt_InitFont(&m_font, reinterpret_cast<const unsigned char *>(m_fontBuffer.data()),
                            stbtt_GetFontOffsetForIndex(reinterpret_cast<const unsigned char *>(m_fontBuffer.data()),
                                                        0)))
        {
            throw BaseException::Create("Failed to init font");
        }
        // 设置字体大小和比例
        m_fontScale = stbtt_ScaleForPixelHeight(&m_font, m_fontSize);
        // 获取字体的位图
        int width, height;
        unsigned char *bitmap = stbtt_GetCodepointBitmap(&m_font, 0, m_fontScale, 'X', &width, &height,
                                                         nullptr, nullptr);
        m_axisNameTextures[0].Upload(bitmap, width, height, TextureFormat::R);
        stbtt_FreeBitmap(bitmap, nullptr);
        bitmap = stbtt_GetCodepointBitmap(&m_font, 0, m_fontScale, 'Y', &width, &height, nullptr, nullptr);
        m_axisNameTextures[1].Upload(bitmap, width, height, TextureFormat::R);
        stbtt_FreeBitmap(bitmap, nullptr);
        bitmap = stbtt_GetCodepointBitmap(&m_font, 0, m_fontScale, 'Z', &width, &height, nullptr, nullptr);
        m_axisNameTextures[2].Upload(bitmap, width, height, TextureFormat::R);
        stbtt_FreeBitmap(bitmap, nullptr);
    }

    ViewportCompass::~ViewportCompass()
    {
        glDeleteProgramPipelines(1, &m_axisPipeline);
        glDeleteVertexArrays(1, &m_axisVAO);
        glDeleteBuffers(1, &m_axisVBO);
    }

    void ViewportCompass::Render(const Camera &camera) const
    {
        glLineWidth(1.5f);

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glViewport(0, 0, 100, 100);

        glBindVertexArray(m_axisVAO);
        glBindProgramPipeline(m_axisPipeline);

        Math::Matrix4F view = camera.ViewMatrix();
        view.Block<3, 1>(0, 3) = Math::Vector3F::Zero();
        m_axisVertexShader.SetMat4("view", view);
        m_axisVertexShader.SetMat4("projection", Orthographic(-1.2f, 1.2f, -1.2f, 1.2f, -2.0f, 2.0f));

        glDrawArrays(GL_LINES, 0, 6);
        glBindVertexArray(0);

        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
    }
}
