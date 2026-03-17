/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "OpenGLTexture2D.h"
#include "Rendering/Renderer.h"
#include "Rendering/OpenGL/OpenGLContext.h"

namespace TG::Rendering
{
    class OpenGLRenderer final : public Renderer
    {
    public:
        explicit OpenGLRenderer(const IDefaultVideoPort& videoPort);
        ~OpenGLRenderer() override;

        GraphicsAPI API() override;

    	[[nodiscard]] const OpenGLContext& GetContext() const override;

        void BeginRender() override;
    	void EndRender() override;

        void Draw(Mesh const* mesh, Material const* material) override;
        void Present() override;

        void ScreenFramebufferResizeCallback(unsigned int width, unsigned int height) override;
        void SceneViewportChangedCallback(int posX, int posY, unsigned int width, unsigned int height) override;

        void RenderToScene() override;
        void RenderToUI() override;

        void RenderToTexture() override;
        Texture const* RenderTarget() override;

    	void SetVSync(bool enable) const override;

        unsigned int SceneWidth() const;
        unsigned int SceneHeight() const;

    private:
        void ResizeFrameBuffer(unsigned int width, unsigned int height);

        OpenGLContext m_context;

        unsigned int m_screenWidth{ 0 };
        unsigned int m_screenHeight{ 0 };

        int m_scenePosX{ 0 };
        int m_scenePosY{ 0 };
        unsigned int m_sceneWidth{ 0 };
        unsigned int m_sceneHeight{ 0 };

        GLuint m_fbo{ 0 };
        OpenGLTexture2D m_framebufferTexture;
        GLuint m_depthStencilBuffer{ 0 };
    };
}
