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
    void OpenGLTexture2D::Upload(std::string_view filePath) const
    {
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
