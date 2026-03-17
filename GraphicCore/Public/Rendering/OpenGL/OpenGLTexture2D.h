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

        void Upload(std::string_view filePath) override;
        void Upload(unsigned char const* data, int width, int height,
            TextureFormat textureFormat) override;

        void Resize(int width, int height) override;
        void FilteringMode(TextureFilteringMode filteringMode) override;
        void WrapMode(TextureWrapMode wrapMode) override;

    private:
        int m_width{ 0 };
        int m_height{ 0 };
        GLint m_internalFormat{ GL_SRGB8_ALPHA8 };
        GLenum m_format{ GL_RGBA };
    };
}
