/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/Texture.h"

namespace TG::Rendering
{
    class OpenGLTexture : public Texture
    {
    public:
        OpenGLTexture();
        ~OpenGLTexture() override;

        [[nodiscard]] TextureID GetID() const override { return m_texture; }

    protected:
        GLuint m_texture{};
    };
}
