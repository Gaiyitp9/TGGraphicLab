/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Rendering/OpenGL/OpenGLTexture2D.h"
#include "Exception/BaseException.h"
#include "stb_image.h"

namespace TG::Rendering
{
    void OpenGLTexture2D::Upload(std::string_view filePath)
    {
        int channels;
        if (unsigned char* data = stbi_load(filePath.data(), &m_width, &m_height, &channels, 0))
        {
            if (channels == 1)
            {
                m_internalFormat = GL_RED;
                m_format = GL_RED;
            }
            else if (channels == 3)
            {
            	m_internalFormat = GL_SRGB8;
                m_format = GL_RGB;
            }
            else
            {
            	m_internalFormat = GL_SRGB8_ALPHA8;
                m_format = GL_RGBA;
            }

            glBindTexture(GL_TEXTURE_2D, m_texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            throw BaseException::Create("Failed to load texture");
        }
    }

    void OpenGLTexture2D::Upload(unsigned char const* data, int width, int height, TextureFormat textureFormat)
    {
        m_width = width;
        m_height = height;
        switch (textureFormat)
        {
            case TextureFormat::R:
            {
                m_internalFormat = GL_RED;
                m_format = GL_RED;
                break;
            }
            case TextureFormat::RG:
            {
                m_internalFormat = GL_RG;
                m_format = GL_RG;
                break;
            }
            case TextureFormat::RGB:
            {
                m_internalFormat = GL_SRGB8;
                m_format = GL_RGB;
                break;
            }
            case TextureFormat::RGBA:
            {
                m_internalFormat = GL_SRGB8_ALPHA8;
                m_format = GL_RGBA;
                break;
            }
            default:
            {
                m_internalFormat = GL_RGBA;
                m_format = GL_RGBA;
            }
        }

        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, data);
    }

    void OpenGLTexture2D::Resize(int width, int height)
    {
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, width, height, 0, m_format, GL_UNSIGNED_BYTE, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGLTexture2D::FilteringMode(TextureFilteringMode filteringMode)
    {
        glBindTexture(GL_TEXTURE_2D, m_texture);
        GLint mode = GL_NEAREST;
        switch (filteringMode)
        {
            case TextureFilteringMode::POINT:
                mode = GL_NEAREST;
                break;
            case TextureFilteringMode::BILINEAR:
                mode = GL_LINEAR;
                break;
            case TextureFilteringMode::TRILINEAR:
                mode = GL_LINEAR_MIPMAP_LINEAR;
                break;
            case TextureFilteringMode::MIN_MAG_POINT_MIPMAP_POINT:
                mode = GL_NEAREST_MIPMAP_NEAREST;
                break;
            case TextureFilteringMode::MIN_MAG_LINEAR_MIPMAP_POINT:
                mode = GL_LINEAR_MIPMAP_NEAREST;
                break;
            case TextureFilteringMode::MIN_MAG_POINT_MIPMAP_LINEAR:
                mode = GL_NEAREST_MIPMAP_LINEAR;
                break;
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGLTexture2D::WrapMode(TextureWrapMode wrapMode)
    {
        glBindTexture(GL_TEXTURE_2D, m_texture);
        GLint mode = GL_REPEAT;
        switch (wrapMode)
        {
            case TextureWrapMode::REPEAT:
                mode = GL_REPEAT;
                break;
            case TextureWrapMode::CLAMP_TO_EDGE:
                mode = GL_CLAMP_TO_EDGE;
                break;
            case TextureWrapMode::CLAMP_TO_BORDER:
                mode = GL_CLAMP_TO_BORDER;
                break;
            case TextureWrapMode::MIRRORED_REPEAT:
                mode = GL_MIRRORED_REPEAT;
                break;
            case TextureWrapMode::MIRROR_CLAMP_TO_EDGE:
                mode = GL_MIRROR_CLAMP_TO_EDGE;
                break;
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
