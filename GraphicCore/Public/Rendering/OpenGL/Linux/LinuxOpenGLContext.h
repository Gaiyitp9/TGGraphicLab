/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Base/Core.h"
#include <EGL/egl.h>
#include <wayland-egl.h>

namespace TG::Rendering
{
    class TG_API LinuxOpenGLContext
    {
    public:
        LinuxOpenGLContext(wl_display* display, wl_surface* surface, unsigned int width, unsigned int height);
        ~LinuxOpenGLContext();

        LinuxOpenGLContext(const LinuxOpenGLContext&) = delete;
        LinuxOpenGLContext& operator=(const LinuxOpenGLContext&) = delete;
        LinuxOpenGLContext(LinuxOpenGLContext&&) = delete;
        LinuxOpenGLContext& operator=(LinuxOpenGLContext&&) = delete;

        void Present() const;
        void MakeCurrent() const;
        void SetVSync(bool enable) const;

    private:
        EGLDisplay m_eglDisplay{nullptr};
        EGLSurface m_eglSurface{nullptr};
        EGLContext m_context{nullptr};
    };
}
