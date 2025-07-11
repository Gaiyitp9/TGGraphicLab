/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/GraphicsAPIDefines.h"
#include <string_view>

namespace TG::Rendering
{
    enum class TextureFormat
    {
        R,
        RG,
        RGB,
        RGBA,
    };

    class Texture
    {
    public:
        virtual ~Texture() = default;

        [[nodiscard]] virtual TextureID GetID() const = 0;
        virtual void Upload(std::string_view filePath) const = 0;
        virtual void Upload(unsigned char* data, int width, int height, TextureFormat textureFormat) const = 0;
    };
}
