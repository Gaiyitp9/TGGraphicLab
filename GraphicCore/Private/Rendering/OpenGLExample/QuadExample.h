/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/Example.h"
#include "Base/CommonInterfaces.h"
#include "Rendering/Mesh/Mesh.h"
#include "Rendering/OpenGL/OpenGLShader.h"

namespace TG::Rendering
{
    class QuadExample final : public Example
    {
    public:
        QuadExample(const IDefaultVideoPort& videoPort, const ITimer& timer);
        ~QuadExample() override;

        void Render() override;

    private:
        const IDefaultVideoPort& m_videoPort;
        const ITimer& m_timer;

        Mesh m_quadMesh;

        GLuint m_VAO{};
        GLuint m_VBO{};
        GLuint m_EBO{};
        bool m_wireframe{ false };
        GLuint m_pipeline{};
        OpenGLShader m_vertexShader;
        OpenGLShader m_fragmentShader;
        OpenGLShader m_wireframeGeometryShader;
        OpenGLShader m_wireframeFragmentShader;
        GLuint m_albedo[2]{};
    };
}
