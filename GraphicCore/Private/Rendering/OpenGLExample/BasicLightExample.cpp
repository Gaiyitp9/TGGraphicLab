/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Rendering/OpenGLExample/BasicLightExample.h"

namespace TG::Rendering
{
    BasicLightExample::BasicLightExample(const std::weak_ptr<IDefaultVideoPort> &videoPort,
        const std::weak_ptr<ITimer> &timer) : m_camera(videoPort, timer),
        m_vertexShader("Shaders/GLSL/LightingModel/Phong/BasicLighting.vert", ShaderStage::Vertex),
        m_fragmentShader("Shaders/GLSL/LightingModel/Phong/Phong.frag", ShaderStage::Fragment)
    {

    }

    BasicLightExample::~BasicLightExample()
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
        glDeleteProgramPipelines(1, &m_pipeline);
    }

    void BasicLightExample::Render()
    {
        m_viewportGrid.Render(m_camera);
        m_viewportCompass.Render(m_camera);
    }
}
