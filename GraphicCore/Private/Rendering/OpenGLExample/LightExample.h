/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/Example.h"
#include "Base/CommonInterfaces.h"
#include "Geometry/Mesh.h"
#include "Rendering/Camera.h"
#include "Rendering/OpenGL/OpenGLShader.h"
#include "Rendering/OpenGL/OpenGLTexture2D.h"
#include "ViewportGrid.h"

namespace TG::Rendering
{
    class LightExample final : public Example
    {
    public:
        LightExample(const std::weak_ptr<IDefaultVideoPort>& videoPort, const std::weak_ptr<ITimer> &timer);
        ~LightExample() override;

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
        OpenGLShader m_vertexShader;
        OpenGLShader m_fragmentShader;
        OpenGLShader m_geometryShader;
        OpenGLTexture2D m_textures[2]{};

        ViewportGrid m_viewportGrid;
    };
}
