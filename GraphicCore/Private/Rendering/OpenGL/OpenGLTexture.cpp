/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Rendering/OpenGL/OpenGLTexture.h"

namespace TG::Rendering
{
    OpenGLTexture::OpenGLTexture()
    {
        glGenTextures(1, &m_texture);
    }

    OpenGLTexture::~OpenGLTexture()
    {
        glDeleteTextures(1, &m_texture);
    }
}
