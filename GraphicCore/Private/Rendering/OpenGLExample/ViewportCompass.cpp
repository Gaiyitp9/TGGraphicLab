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
        : m_axisVertexShader("Assets/Shaders/GLSL/viewport_compass.vert", ShaderStage::Vertex),
          m_axisFragmentShader("Assets/Shaders/GLSL/viewport_compass.frag", ShaderStage::Fragment),
          m_axisVertices{
              // 位置           颜色
              0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
              1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
              0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
              0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
              0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
              0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
          },
          m_axisNameVertexShader("Assets/Shaders/GLSL/compass_name.vert", ShaderStage::Vertex),
          m_axisNameFragmentShader("Assets/Shaders/GLSL/compass_name.frag", ShaderStage::Fragment),
          m_fontBuffer(LoadBinaryFile("Assets/Fonts/consola.ttf"))
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

        m_axisNameMesh = CreatePrimitive(PrimitiveType::Quad);

        glGenBuffers(1, &m_axisNameVBO);
        glGenBuffers(1, &m_axisNameEBO);
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

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_axisNameEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(sizeof(std::uint32_t) * m_axisNameMesh.indices.size()),
            m_axisNameMesh.indices.data(), GL_STATIC_DRAW);
        glBindVertexArray(0);

        // 初始化字体
        if (!stbtt_InitFont(&m_font, reinterpret_cast<const unsigned char *>(m_fontBuffer.data()),
                            stbtt_GetFontOffsetForIndex(reinterpret_cast<const unsigned char *>(m_fontBuffer.data()),
                                                        0)))
        {
            throw BaseException::Create("Failed to init font");
        }

        // 保存当前对齐设置
        GLint prevAlignment;
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &prevAlignment);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // 设置字体大小和比例
        m_fontScale = stbtt_ScaleForPixelHeight(&m_font, m_fontSize);
        // 获取字体的位图
        int width, height;
        unsigned char* bitmap = stbtt_GetCodepointBitmap(&m_font, 0, m_fontScale, 'X', &width, &height,
                                                         nullptr, nullptr);
        m_axisNameTextures[0].Upload(bitmap, width, height, TextureFormat::R);
        stbtt_FreeBitmap(bitmap, nullptr);
        bitmap = stbtt_GetCodepointBitmap(&m_font, 0, m_fontScale, 'Y', &width, &height, nullptr, nullptr);
        m_axisNameTextures[1].Upload(bitmap, width, height, TextureFormat::R);
        stbtt_FreeBitmap(bitmap, nullptr);
        bitmap = stbtt_GetCodepointBitmap(&m_font, 0, m_fontScale, 'Z', &width, &height, nullptr, nullptr);
        m_axisNameTextures[2].Upload(bitmap, width, height, TextureFormat::R);
        stbtt_FreeBitmap(bitmap, nullptr);

        // 恢复原来的对齐设置
        glPixelStorei(GL_UNPACK_ALIGNMENT, prevAlignment);
    }

    ViewportCompass::~ViewportCompass()
    {
        glDeleteProgramPipelines(1, &m_axisPipeline);
        glDeleteVertexArrays(1, &m_axisVAO);
        glDeleteBuffers(1, &m_axisVBO);
    }

    void ViewportCompass::Render(const Camera &camera) const
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glViewport(0, 0, 100, 100);
        // 绘制坐标轴
        glBindVertexArray(m_axisVAO);
        glBindProgramPipeline(m_axisPipeline);

        Math::Matrix4f view = camera.ViewMatrix();
        view.Block<3, 1>(0, 3) = Math::Vector3f::Zero();
        Math::Matrix4f projection = Orthographic(-1.5f, 1.5f, -1.5f, 1.5f, -2.0f, 2.0f);
        m_axisVertexShader.SetMat4("view", view);
        m_axisVertexShader.SetMat4("projection", projection);

        glDrawArrays(GL_LINES, 0, 6);

        // 绘制坐标轴字母
        glBindVertexArray(m_axisNameVAO);
        glBindProgramPipeline(m_axisNamePipeline);

        m_axisNameVertexShader.SetMat4("view", view);
        m_axisNameVertexShader.SetMat4("projection", projection);

        Math::Matrix4f model = Math::Matrix4f::Identity();
        Math::Vector3f front = view.Block<1, 3>(2, 0).Transpose();
        Math::Vector3f up{ 0.0f, 1.0f, 0.0f };
        Math::Vector3f right = up.Cross(front).Normalized();
        up = front.Cross(right);
        model.Block<3, 1>(0, 0) = right * 0.3f;
        model.Block<3, 1>(0, 1) = up * 0.3f;
        model.Block<3, 1>(0, 2) = front * 0.3f;

        model[0, 3] = 1.25f;
        m_axisNameVertexShader.SetMat4("model", model);
        m_axisNameFragmentShader.SetFloat3("fontColor", 1.0f, 0.0f, 0.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, CastID<GLuint>(m_axisNameTextures[0].GetID()));
    	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_axisNameMesh.indices.size()), GL_UNSIGNED_INT, nullptr);

        model[0, 3] = 0.0f;
        model[1, 3] = 1.25f;
        m_axisNameVertexShader.SetMat4("model", model);
        m_axisNameFragmentShader.SetFloat3("fontColor", 0.0f, 1.0f, 0.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, CastID<GLuint>(m_axisNameTextures[1].GetID()));
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_axisNameMesh.indices.size()), GL_UNSIGNED_INT, nullptr);

        model[0, 3] = 0.0f;
        model[1, 3] = 0.0f;
        model[2, 3] = 1.25f;
        m_axisNameVertexShader.SetMat4("model", model);
        m_axisNameFragmentShader.SetFloat3("fontColor", 0.0f, 0.0f, 1.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, CastID<GLuint>(m_axisNameTextures[2].GetID()));
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_axisNameMesh.indices.size()), GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);

        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
    }
}
