/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/Camera.h"
#include "Rendering/OpenGL/OpenGLShader.h"

namespace TG::Rendering
{
    class ViewportGrid
    {
    public:
        ViewportGrid();
        ~ViewportGrid();

        void Render(const Camera& camera) const;

    private:
        GLuint m_VAO{};
        GLuint m_pipeline{};
        OpenGLShader m_vertexShader;
        OpenGLShader m_fragmentShader;
        OpenGLShader m_geometryShader;
    };
}
