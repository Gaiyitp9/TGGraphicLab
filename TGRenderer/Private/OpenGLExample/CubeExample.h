/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Example.h"
#include "Rendering/Renderer.h"
#include "Rendering/Mesh/Mesh.h"
#include "Rendering/Camera.h"
#include "Rendering/OpenGL/OpenGLShader.h"
#include "Rendering/OpenGL/OpenGLTexture2D.h"
#include "Rendering/Gizmo/ViewportGrid.h"
#include "Rendering/Gizmo/SkyBox.h"
#include "Rendering/Gizmo/ViewportCompass.h"

namespace TG
{
    class CubeExample final : public Example
    {
    public:
        CubeExample(const Rendering::Renderer* renderer, const ITimer& timer);
        ~CubeExample() override;

        void Draw() override;
        void DrawUI() override;

        void OnViewportChanged(unsigned width, unsigned height) override;

    private:
        Camera m_camera;

        Rendering::Mesh m_cubeMesh;
        Math::Vector3f m_cubePositions[10];

        GLuint m_VAO{};
        GLuint m_VBO{};
        GLuint m_EBO{};
        bool m_wireframe{ false };
        GLuint m_pipeline{};
        Rendering::OpenGLShader m_vertexShader;
        Rendering::OpenGLShader m_fragmentShader;
        Rendering::OpenGLShader m_wireframeGeometryShader;
        Rendering::OpenGLShader m_wireframeFragmentShader;
        Rendering::OpenGLTexture2D m_textures[2]{};

        Rendering::SkyBox m_skyBox;
        Rendering::ViewportGrid m_viewportGrid;
        Rendering::ViewportCompass m_viewportCompass;
    };
}
