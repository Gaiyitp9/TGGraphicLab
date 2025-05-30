/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "TextureGL.h"

namespace TG
{
    TextureGL::TextureGL()
    {
        glGenTextures(1, &m_texture);
    }

    TextureGL::~TextureGL()
    {
        glDeleteTextures(1, &m_texture);
    }
}
