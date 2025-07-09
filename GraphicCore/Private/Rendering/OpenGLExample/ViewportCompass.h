/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/Camera.h"
#include "Rendering/OpenGL/OpenGLShader.h"
#include <array>

namespace TG::Rendering
{
    class ViewportCompass
    {
    public:
        ViewportCompass();
        ~ViewportCompass();

        void Render(const Camera& camera) const;

    private:
        GLuint m_VBO{};
        GLuint m_VAO{};
        GLuint m_pipeline{};

        OpenGLShader m_vertexShader;
        OpenGLShader m_fragmentShader;

        std::array<float, 36> m_axisVertices;
    };
}
