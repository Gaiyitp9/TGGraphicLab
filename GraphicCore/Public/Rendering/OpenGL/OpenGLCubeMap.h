/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/OpenGL/OpenGLTexture.h"

namespace TG::Rendering
{
    class OpenGLCubeMap final : public OpenGLTexture
    {
    public:
        OpenGLCubeMap() = default;
        ~OpenGLCubeMap() override = default;

        void Upload(std::string_view filePath) override;
        void Upload(unsigned char const* data, int width, int height,
            TextureFormat textureFormat) override;

        void Resize(int width, int height) override;
        void FilteringMode(TextureFilteringMode filteringMode) override;
        void WrapMode(TextureWrapMode wrapMode) override;
    };
}
