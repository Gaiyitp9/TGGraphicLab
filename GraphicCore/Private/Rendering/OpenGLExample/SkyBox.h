/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/Camera.h"
#include "Rendering/OpenGL/OpenGLShader.h"
#include "Rendering/OpenGL/OpenGLCubeMap.h"
#include "Rendering/Mesh/Mesh.h"

namespace TG::Rendering
{
    class SkyBox
    {
    public:
        SkyBox();
        ~SkyBox();

        void Render(const Camera& camera) const;

    private:
        Mesh m_cubeMesh;
        OpenGLCubeMap m_cubeMap;

        GLuint m_VBO{};
        GLuint m_EBO{};
        GLuint m_VAO{};
        GLuint m_pipeline{};

        OpenGLShader m_vertexShader;
        OpenGLShader m_fragmentShader;
    };
}
