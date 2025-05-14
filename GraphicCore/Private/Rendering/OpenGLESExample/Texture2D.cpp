/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Texture2D.h"
#include "stb_image.h"
#include "Exception/BaseException.h"

namespace TG
{
    Texture2D::Texture2D(std::string_view filePath)
    {
        LoadTexture(filePath);
    }

    Texture2D::~Texture2D() = default;

    void Texture2D::Upload(std::string_view filePath) const
    {
        LoadTexture(filePath);
    }

    void Texture2D::LoadTexture(std::string_view filePath) const
    {
        if (filePath.empty())
            return;

        int width, height, channels;
        unsigned char* data = stbi_load(filePath.data(), &width, &height, &channels, 0);
        if (data)
        {
            GLenum format;
            if (channels == 1)
                format = GL_RED;
            else if (channels == 3)
                format = GL_RGB;
            else
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, m_texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
        }
        else
        {
            throw BaseException::Create("Failed to load texture");
        }
    }
}
