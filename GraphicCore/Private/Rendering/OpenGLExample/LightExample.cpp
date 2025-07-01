/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Rendering/OpenGLExample/LightExample.h"

namespace TG::Rendering
{
    LightExample::LightExample(const std::weak_ptr<IDefaultVideoPort> &videoPort, const std::weak_ptr<ITimer> &timer)
        : m_camera(videoPort, timer),
        m_vertexShader("Shaders/GLSL/Cube.vert", ShaderStage::Vertex),
        m_fragmentShader("Shaders/GLSL/Cube.frag", ShaderStage::Fragment),
        m_geometryShader("Shaders/GLSL/Wireframe.geom", ShaderStage::Geometry)
    {

    }


}
