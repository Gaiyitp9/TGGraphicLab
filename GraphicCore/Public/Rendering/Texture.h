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

    enum class TextureFilteringMode
    {
        POINT,
        BILINEAR,
        TRILINEAR,
        MIN_MAG_POINT_MIPMAP_POINT,
        MIN_MAG_LINEAR_MIPMAP_POINT,
        MIN_MAG_POINT_MIPMAP_LINEAR,
    };

    enum class TextureWrapMode
    {
        REPEAT,
        CLAMP_TO_EDGE,
        CLAMP_TO_BORDER,
        MIRRORED_REPEAT,
        MIRROR_CLAMP_TO_EDGE,
    };

    class Texture
    {
    public:
        virtual ~Texture() = default;

        [[nodiscard]] virtual TextureID GetID() const = 0;
        virtual void Upload(std::string_view filePath) = 0;
        virtual void Upload(unsigned char const* data, int width, int height,
            TextureFormat textureFormat) = 0;

        virtual void Resize(int width, int height) = 0;
        virtual void FilteringMode(TextureFilteringMode filteringMode) = 0;
        virtual void WrapMode(TextureWrapMode wrapMode) = 0;
    };
}
