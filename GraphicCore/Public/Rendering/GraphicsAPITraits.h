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
    using OpenGLID = GLuint;

    using ShaderID = std::variant<OpenGLID>;
}
