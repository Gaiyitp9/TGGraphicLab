/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Example.h"
#include "DynamicGles.h"
#include "Base/Timer.h"

namespace TG
{
    class QuadExample final : public Example
    {
    public:
        explicit QuadExample(const ITimer& timer);
        ~QuadExample() override;

        void Render() override;

    private:
        const ITimer& m_timer;
        GLuint m_shaderProgram{};
        GLuint m_shaderProgramWireframe{};
        GLuint m_VAO{};
        GLuint m_VBO{};
        GLuint m_EBO{};
        bool m_wireframe{ false };
    };
}
