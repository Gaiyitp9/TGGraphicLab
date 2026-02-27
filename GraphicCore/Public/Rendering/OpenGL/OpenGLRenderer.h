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

    	char const* Type() override;

    	[[nodiscard]] const IDefaultVideoPort& VideoPort() const override;

        void PreRender() override;
        void Draw(Mesh const* mesh, Material const* material) override;
        void Present() override;
        void ScreenFrameBufferResizeCallback(unsigned int width, unsigned int height) override;
        void SceneFrameBufferResizeCallback(unsigned int width, unsigned int height) override;
        void RenderToTexture() const override;
        void RenderToScreen() const override;

        Texture const* RenderTarget() override;

    	void SetVSync(bool enable) const override;

    private:
        void ResizeFrameBuffer(unsigned int width, unsigned int height);

    	const IDefaultVideoPort& m_videoPort;

        OpenGLContext m_context;

        unsigned int m_screenWidth{ 0 };
        unsigned int m_screenHeight{ 0 };

        GLuint m_fbo{ 0 };
        OpenGLTexture2D m_framebufferTexture;
        GLuint m_depthStencilBuffer{ 0 };

        unsigned int m_sceneWidth{ 0 };
        unsigned int m_sceneHeight{ 0 };
    };
}
