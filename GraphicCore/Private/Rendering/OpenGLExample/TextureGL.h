/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "glad/gl.h"

namespace TG
{
    class TextureGL
    {
    public:
        TextureGL();
        ~TextureGL();

        [[nodiscard]] GLuint GetID() const { return m_texture; }

    protected:
        GLuint m_texture{};
    };
}
