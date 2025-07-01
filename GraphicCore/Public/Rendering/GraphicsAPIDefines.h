/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "glad/gl.h"
#include <variant>

namespace TG::Rendering
{
    enum class GraphicsAPI : unsigned char
    {
        OpenGLES,
        OpenGL,
        Vulkan,
        DirectX11,
        DirectX12,
    };

    using OpenGLID = GLuint;

    using ShaderID = std::variant<OpenGLID>;
    using TextureID = std::variant<OpenGLID>;

    template<typename T>
    T CastID(ShaderID id)
    {
        return std::get<T>(id);
    }
}
