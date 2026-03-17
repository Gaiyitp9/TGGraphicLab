/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Context.h"
#include "Rendering/Mesh/Mesh.h"
#include "Rendering/Material.h"

namespace TG::Rendering
{
    class Texture;

    class Renderer
    {
    public:
        virtual ~Renderer() = default;

    	virtual GraphicsAPI API() = 0;

    	virtual const Context& GetContext() const = 0;

        virtual void BeginRender() = 0;
    	virtual void EndRender() = 0;

        virtual void Draw(Mesh const* mesh, Material const* material) = 0;
        virtual void Present() = 0;

        virtual void ScreenFramebufferResizeCallback(unsigned int width, unsigned int height) = 0;
        virtual void SceneViewportChangedCallback(int posX, int posY, unsigned int width, unsigned int height) = 0;

        virtual void RenderToScene() = 0;
    	virtual void RenderToUI() = 0;

        virtual void RenderToTexture() = 0;
        virtual Texture const* RenderTarget() = 0;

    	virtual void SetVSync(bool enable) const = 0;
    };
}
