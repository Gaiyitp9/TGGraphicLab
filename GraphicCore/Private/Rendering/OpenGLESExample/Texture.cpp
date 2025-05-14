/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Texture.h"

namespace TG
{
    Texture::Texture()
    {
        glGenTextures(1, &m_texture);
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &m_texture);
    }
}
