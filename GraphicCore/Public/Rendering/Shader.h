/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Core.hpp"
#include "glm/matrix.hpp"
#include "Rendering/Renderer.h"
#include <string_view>

namespace TG::Rendering
{
    enum class ShaderStage : unsigned char
    {
        Vertex,
        Geometry,
        Fragment,
    };

    template<GraphicsAPI API>
    class Shader
    {
    public:
        Shader(std::string_view shaderPath, ShaderStage stage);
        ~Shader();

        // [[nodiscard]] GLuint GetID() const { return m_shaderID; }
        void SetBool(std::string_view name, bool value) const;
        void SetInt(std::string_view name, int value) const;
        void SetInt2(std::string_view name, int v0, int v1) const;
        void SetFloat(std::string_view name, float value) const;
        void SetFloat2(std::string_view name, float v0, float v1) const;
        void SetFloat3(std::string_view name, float v0, float v1, float v3) const;
        void SetFloat4(std::string_view name, float v0, float v1, float v2, float v3) const;
        void SetMat4(std::string_view name, const Math::Matrix4F& value) const;
        void SetMat4(std::string_view name, const glm::mat4& value) const;
    };
}
