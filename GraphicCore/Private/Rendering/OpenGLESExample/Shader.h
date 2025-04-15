/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "DynamicGles.h"

namespace TG
{
    enum class ShaderStage : unsigned char
    {
        Vertex,
        Geometry,
        Fragment,
    };

    class Shader
    {
    public:
        Shader(std::string_view shaderPath, ShaderStage shaderStage);
        ~Shader();

        void Activate() const;
        void Deactivate() const;
        [[nodiscard]] GLuint GetId() const { return m_shaderId; }
        void SetBool(std::string_view name, bool value) const;
        void SetInt(std::string_view name, int value) const;
        void SetFloat(std::string_view name, float value) const;
        void SetFloat4(std::string_view name, float v0, float v1, float v2, float v3) const;

    private:
        GLuint m_shaderId{};
    };
}
