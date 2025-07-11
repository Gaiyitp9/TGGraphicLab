/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/Camera.h"
#include "Rendering/OpenGL/OpenGLShader.h"
#include "Rendering/OpenGL/OpenGLTexture2D.h"
#include "Geometry/Primitives.h"
#include "stb_truetype.h"
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
        OpenGLShader m_axisVertexShader;
        OpenGLShader m_axisFragmentShader;

        std::array<float, 36> m_axisVertices;

        GLuint m_axisVBO{};
        GLuint m_axisVAO{};
        GLuint m_axisPipeline{};

        OpenGLShader m_axisNameVertexShader;
        OpenGLShader m_axisNameFragmentShader;
        Geometry::Mesh m_axisNameMesh;

        GLuint m_axisNameVBO{};
        GLuint m_axisNameVAO{};
        GLuint m_axisNamePipeline{};

        std::vector<char> m_fontBuffer;
        stbtt_fontinfo m_font{};
        float m_fontSize = 20.0f;
        float m_fontScale;

        OpenGLTexture2D m_axisNameTextures[3];
    };
}
