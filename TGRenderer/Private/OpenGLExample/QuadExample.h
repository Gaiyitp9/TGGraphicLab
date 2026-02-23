/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Example.h"
#include "Base/CommonInterfaces.h"
#include "Rendering/Mesh/Mesh.h"
#include "Rendering/OpenGL/OpenGLShader.h"

namespace TG
{
    class QuadExample final : public Example
    {
    public:
        QuadExample(const IDefaultVideoPort& videoPort, const ITimer& timer);
        ~QuadExample() override;

        void Draw() override;
        void DrawUI() override;

    private:
        const IDefaultVideoPort& m_videoPort;
        const ITimer& m_timer;

        Rendering::Mesh m_quadMesh;

        GLuint m_VAO{};
        GLuint m_VBO{};
        GLuint m_EBO{};
        bool m_wireframe{ false };
        GLuint m_pipeline{};
        Rendering::OpenGLShader m_vertexShader;
        Rendering::OpenGLShader m_fragmentShader;
        Rendering::OpenGLShader m_wireframeGeometryShader;
        Rendering::OpenGLShader m_wireframeFragmentShader;
        GLuint m_albedo[2]{};
    };
}
