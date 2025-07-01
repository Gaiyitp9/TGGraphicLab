/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/GraphicsAPIDefines.h"
#include "Math/Core.hpp"
#include "glm/matrix.hpp"
#include <string_view>

namespace TG::Rendering
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
        virtual ~Shader() = default;

        [[nodiscard]] virtual ShaderID GetID() const = 0;

        virtual void SetBool(std::string_view name, bool value) const = 0;
        virtual void SetInt(std::string_view name, int value) const = 0;
        virtual void SetInt2(std::string_view name, int v0, int v1) const = 0;
        virtual void SetFloat(std::string_view name, float value) const = 0;
        virtual void SetFloat2(std::string_view name, float v0, float v1) const = 0;
        virtual void SetFloat3(std::string_view name, float v0, float v1, float v3) const = 0;
        virtual void SetFloat4(std::string_view name, float v0, float v1, float v2, float v3) const = 0;
        virtual void SetMat4(std::string_view name, const Math::Matrix4F& value) const = 0;
        virtual void SetMat4(std::string_view name, const glm::mat4& value) const = 0;
    };
}
