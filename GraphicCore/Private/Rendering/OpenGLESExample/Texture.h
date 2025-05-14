/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "DynamicGles.h"

namespace TG
{
    class Texture
    {
    public:
        Texture();
        ~Texture();

        [[nodiscard]] GLuint GetID() const { return m_texture; }

    protected:
        GLuint m_texture{};
    };
}
