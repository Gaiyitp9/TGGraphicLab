/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Rendering/OpenGL/OpenGLCubeMap.h"
#include "Exception/BaseException.h"
#include "Base/Utility.h"
#include "stb_image.h"
#include <array>
#include <filesystem>

namespace TG::Rendering
{
    void OpenGLCubeMap::Upload(std::string_view filePath) const
    {
        std::array<std::string, 6> faces;

        std::filesystem::path cubeMapsDir(filePath);
        if (!is_directory(cubeMapsDir))
            throw BaseException::Create(std::format("{} is not a cube map directory.", filePath));

        unsigned int loadCount = 0;
        for (const auto& entry : std::filesystem::directory_iterator(cubeMapsDir))
        {
            if (entry.is_regular_file() && IsImageByExtension(entry.path().extension().string()))
            {
                std::string originPath = entry.path().string();
                std::string lowerPath = ToLower(originPath);
                if (lowerPath.contains("positive") && lowerPath.contains("x") && faces[0].empty())
                {
                    faces[0] = originPath;
                    loadCount++;
                }
                if (lowerPath.contains("negative") && lowerPath.contains("x") && faces[1].empty())
                {
                    faces[1] = originPath;
                    loadCount++;
                }
                if (lowerPath.contains("positive") && lowerPath.contains("y") && faces[2].empty())
                {
                    faces[2] = originPath;
                    loadCount++;
                }
                if (lowerPath.contains("negative") && lowerPath.contains("y") && faces[3].empty())
                {
                    faces[3] = originPath;
                    loadCount++;
                }
                if (lowerPath.contains("positive") && lowerPath.contains("z") && faces[4].empty())
                {
                    faces[4] = originPath;
                    loadCount++;
                }
                if (lowerPath.contains("negative") && lowerPath.contains("z") && faces[5].empty())
                {
                    faces[5] = originPath;
                    loadCount++;
                }
            }
        }

        if (loadCount != 6)
            throw BaseException::Create(std::format("{} doesn't contain 6 cube map faces.", filePath));

        glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        int width, height, channels;
        for (unsigned int i = 0; i < faces.size(); ++i)
        {
            if (unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0))
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

                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0,
                    format, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                throw BaseException::Create("Failed to load texture");
            }
        }
    }
}
