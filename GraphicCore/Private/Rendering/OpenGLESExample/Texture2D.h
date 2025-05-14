/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Texture.h"
#include <string_view>

namespace TG
{
    class Texture2D : public Texture
    {
    public:
        Texture2D() = default;
        explicit Texture2D(std::string_view filePath);
        ~Texture2D();

        void Upload(std::string_view filePath) const;

    private:
        void LoadTexture(std::string_view filePath) const;
    };
}
