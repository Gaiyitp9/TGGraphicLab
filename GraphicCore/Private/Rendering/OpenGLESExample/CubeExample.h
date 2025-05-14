/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Example.h"
#include "DynamicGles.h"
#include "Base/Timer.h"
#include "Base/WindowBase.hpp"
#include "Geometry/Mesh.h"
#include "Shader.h"
#include "Texture2D.h"

namespace TG
{
    class CubeExample final : public Example
    {
    public:
        explicit CubeExample(const IDefaultVideoPort& videoPort, const ITimer& timer);
        ~CubeExample() override;

        void Render() override;

    private:
        const ITimer& m_timer;
        const IDefaultVideoPort& m_videoPort;

        Geometry::Mesh m_cubeMesh;
        glm::vec3 m_cubePositions[10]{};

        GLuint m_VAO{};
        GLuint m_VBO{};
        GLuint m_EBO{};
        bool m_wireframe{ false };
        GLuint m_pipeline{};
        Shader m_vertexShader;
        Shader m_fragmentShader;
        Shader m_geometryShader;
        Texture2D m_textures[2]{};
    };
}
