/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/Renderer.h"
#include "Base/CommonInterfaces.h"
#include "DynamicGles.h"
#include "Example.h"
#include <memory>

namespace TG
{
    class OpenGLESRenderer final : public Renderer
    {
    public:
        OpenGLESRenderer(const std::weak_ptr<IDefaultVideoPort>& videoPort, const std::weak_ptr<ITimer>& timer);
        ~OpenGLESRenderer() override;

        void Render() override;
        void Present() override;
        void FrameBufferResizeCallback(unsigned int width, unsigned int height) override;

    private:
        EGLDisplay m_eglDisplay{ EGL_NO_DISPLAY };
        EGLContext m_eglContext{ EGL_NO_CONTEXT };
        EGLSurface m_eglSurface{ EGL_NO_SURFACE };
        bool m_getDisplay{ false };
        bool m_createContext{ false };
        bool m_createSurface{ false };

        std::unique_ptr<Example> m_example;
    };
}
