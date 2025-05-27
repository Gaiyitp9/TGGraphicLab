/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/Camera.h"
#include "Shader.h"

namespace TG
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
        Shader m_vertexShader;
        Shader m_fragmentShader;
        Shader m_geometryShader;
    };
}
