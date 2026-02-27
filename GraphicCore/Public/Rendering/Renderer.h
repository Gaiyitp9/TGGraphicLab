/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/Mesh/Mesh.h"
#include "Rendering/Material.h"
#include "Base/CommonInterfaces.h"

namespace TG::Rendering
{
    class Texture;

    class Renderer
    {
    public:
        virtual ~Renderer() = default;

    	virtual char const* Type() = 0;

    	[[nodiscard]] virtual const IDefaultVideoPort& VideoPort() const = 0;

        virtual void PreRender() = 0;

        virtual void Draw(Mesh const* mesh, Material const* material) = 0;
        virtual void Present() = 0;

        virtual void ScreenFrameBufferResizeCallback(unsigned int width, unsigned int height) = 0;
        virtual void SceneFrameBufferResizeCallback(unsigned int width, unsigned int height) = 0;
        virtual void RenderToTexture() const = 0;
        virtual void RenderToScreen() const = 0;

        virtual Texture const* RenderTarget() = 0;

    	virtual void SetVSync(bool enable) const = 0;
    };
}
