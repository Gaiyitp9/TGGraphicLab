/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/OpenGL/OpenGLTexture.h"

namespace TG::Rendering
{
    class OpenGLTexture2D : public OpenGLTexture
    {
    public:
        OpenGLTexture2D() = default;
        ~OpenGLTexture2D() override = default;

        void Upload(std::string_view filePath) const override;
        void Upload(unsigned char* data, int width, int height, TextureFormat textureFormat) const override;
    };
}
