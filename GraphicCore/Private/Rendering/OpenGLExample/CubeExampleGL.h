/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "../Example.h"
#include "Base/CommonInterfaces.h"
#include "Geometry/Mesh.h"
#include "Rendering/Camera.h"
#include "ShaderGL.h"
#include "Texture2DGL.h"
#include "ViewportGridGL.h"

namespace TG
{
    class CubeExampleGL final : public Example
    {
    public:
        CubeExampleGL(const std::weak_ptr<IDefaultVideoPort>& videoPort, const std::weak_ptr<ITimer> &timer);
        ~CubeExampleGL() override;

        void Render() override;

    private:
        Camera m_camera;

        Geometry::Mesh m_cubeMesh;
        Math::Vector3F m_cubePositions[10];

        GLuint m_VAO{};
        GLuint m_VBO{};
        GLuint m_EBO{};
        bool m_wireframe{ false };
        GLuint m_pipeline{};
        ShaderGL m_vertexShader;
        ShaderGL m_fragmentShader;
        ShaderGL m_geometryShader;
        Texture2DGL m_textures[2]{};

        ViewportGridGL m_viewportGrid;
    };
}
