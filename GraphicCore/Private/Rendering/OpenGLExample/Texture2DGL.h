/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "TextureGL.h"
#include <string_view>

namespace TG
{
    class Texture2DGL : public TextureGL
    {
    public:
        Texture2DGL() = default;
        explicit Texture2DGL(std::string_view filePath);
        ~Texture2DGL();

        void Upload(std::string_view filePath) const;

    private:
        void LoadTexture(std::string_view filePath) const;
    };
}
