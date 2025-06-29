/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/Shader.h"

namespace TG::Rendering
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(std::string_view shaderPath, ShaderStage shaderStage);
        ~OpenGLShader() override;

        [[nodiscard]] ShaderID GetID() const override { return m_shader; }

        void SetBool(std::string_view name, bool value) const override;
        void SetInt(std::string_view name, int value) const override;
        void SetInt2(std::string_view name, int v0, int v1) const override;
        void SetFloat(std::string_view name, float value) const override;
        void SetFloat2(std::string_view name, float v0, float v1) const override;
        void SetFloat3(std::string_view name, float v0, float v1, float v3) const override;
        void SetFloat4(std::string_view name, float v0, float v1, float v2, float v3) const override;
        void SetMat4(std::string_view name, const Math::Matrix4F& value) const override;
        void SetMat4(std::string_view name, const glm::mat4& value) const override;

    private:
        GLuint m_shader{};
    };
}
