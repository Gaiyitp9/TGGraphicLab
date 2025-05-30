/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Texture2DGL.h"
#include "stb_image.h"
#include "Exception/BaseException.h"

namespace TG
{
    Texture2DGL::Texture2DGL(std::string_view filePath)
    {
        LoadTexture(filePath);
    }

    Texture2DGL::~Texture2DGL() = default;

    void Texture2DGL::Upload(std::string_view filePath) const
    {
        LoadTexture(filePath);
    }

    void Texture2DGL::LoadTexture(std::string_view filePath) const
    {
        if (filePath.empty())
            return;

        int width, height, channels;
        if (unsigned char* data = stbi_load(filePath.data(), &width, &height, &channels, 0))
        {
            GLint internalFormat;
            GLenum format;
            if (channels == 1)
            {
                internalFormat = GL_RED;
                format = GL_RED;
            }
            else if (channels == 3)
            {
                internalFormat = GL_RGB;
                format = GL_RGB;
            }
            else
            {
                internalFormat = GL_RGBA;
                format = GL_RGBA;
            }

            glBindTexture(GL_TEXTURE_2D, m_texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
        }
        else
        {
            throw BaseException::Create("Failed to load texture");
        }
    }
}
