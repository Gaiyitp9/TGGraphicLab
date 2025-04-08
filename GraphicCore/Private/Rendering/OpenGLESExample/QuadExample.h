/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Example.h"
#include "DynamicGles.h"

namespace TG
{
    class QuadExample : public Example
    {
    public:
        QuadExample();
        ~QuadExample() override;

        void Render() override;

    private:
        GLuint m_shaderProgram{};
        GLuint m_shaderProgramWireframe{};
        GLuint m_VAO{};
        GLuint m_VBO{};
        GLuint m_EBO{};
        bool m_wireframe{ false };
    };
}
