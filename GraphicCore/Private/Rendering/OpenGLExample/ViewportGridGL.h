/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/Camera.h"
#include "ShaderGL.h"

namespace TG
{
    class ViewportGridGL
    {
    public:
        ViewportGridGL();
        ~ViewportGridGL();

        void Render(const Camera& camera) const;

    private:
        GLuint m_VAO{};
        GLuint m_pipeline{};
        ShaderGL m_vertexShader;
        ShaderGL m_fragmentShader;
        ShaderGL m_geometryShader;
    };
}
