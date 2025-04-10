/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "DynamicGles.h"

namespace TG
{
    class Shader
    {
    public:
        void Use();
        void SetBool(std::string_view name, bool value) const;
        void SetInt(std::string_view name, int value) const;
        void SetFloat(std::string_view name, float value) const;

    private:
        GLuint m_shaderProgram{};
    };
}
