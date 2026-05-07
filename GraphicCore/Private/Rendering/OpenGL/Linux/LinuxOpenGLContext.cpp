/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Rendering/OpenGL/Linux/LinuxOpenGLContext.h"
#include "Exception/Linux/EGLException.h"
#include "Diagnostic/Log.hpp"
#include "glad/gl.h"

namespace TG::Rendering
{
    LinuxOpenGLContext::LinuxOpenGLContext(wl_display* display, wl_surface* surface,
        unsigned int width, unsigned int height)
    {
        m_eglDisplay = eglGetDisplay(display);
        if (m_eglDisplay == EGL_NO_DISPLAY)
            throw EGLException::Create("Failed to get EGL display");

        // 1. 初始化egl
        int major, minor;
        if (!eglInitialize(m_eglDisplay, &major, &minor))
            throw EGLException::Create("Failed to initialize EGL");
        LogInfo("EGL version: {}.{}", major, minor);

        // 2. 绑定API
        if (!eglBindAPI(EGL_OPENGL_API))
            throw EGLException::Create("Failed to bind OpenGL API");

        // 3. 选择配置
        EGLint configAttribs[]{
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_DEPTH_SIZE, 24,
            EGL_NONE
        };
        EGLConfig config;
        EGLint num_configs;
        if (!eglChooseConfig(m_eglDisplay, configAttribs, &config, 1, &num_configs))
            throw EGLException::Create("Failed to get a list of EGL frame buffer configurations");

        // 4. 创建OpenGL 4.6 Context
        EGLint ctxAttribs[] = {
            EGL_CONTEXT_MAJOR_VERSION, 4,
            EGL_CONTEXT_MINOR_VERSION, 6,
            EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
            EGL_NONE
        };
        m_context = eglCreateContext(m_eglDisplay, config, EGL_NO_CONTEXT, ctxAttribs);
        if (m_context == EGL_NO_CONTEXT)
            throw EGLException::Create("Failed to create EGL context");

        wl_egl_window* eglWindow = wl_egl_window_create(surface, width, height);
        // 6. 创建Surface
        m_eglSurface = eglCreateWindowSurface(
            m_eglDisplay,
            config,
            reinterpret_cast<EGLNativeWindowType>(eglWindow),
            nullptr
        );
        if (m_eglSurface == EGL_NO_SURFACE)
            throw EGLException::Create("Failed to create EGL surface");

        if (!eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_context))
            throw EGLException::Create("EGL make current failed");

        // 加载OpenGL函数
        gladLoaderLoadGL();

        // 开启垂直同步
        SetVSync(true);

        // 启用sRGB缓冲
        glEnable(GL_FRAMEBUFFER_SRGB);
    }

    LinuxOpenGLContext::~LinuxOpenGLContext()
    {
        eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroySurface(m_eglDisplay, m_eglSurface);
        eglDestroyContext(m_eglDisplay, m_context);
        eglTerminate(m_eglDisplay);
    }

    void LinuxOpenGLContext::Present() const
    {
        if (!eglSwapBuffers(m_eglDisplay, m_eglSurface))
            throw EGLException::Create("EGL swap buffers failed");
    }

    void LinuxOpenGLContext::MakeCurrent() const
    {
        if (!eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_context))
            throw EGLException::Create("EGL make current failed");
    }

    void LinuxOpenGLContext::SetVSync(bool enable) const
    {
        if (!eglSwapInterval(m_eglDisplay, enable ? 1 : 0))
            throw EGLException::Create("EGL set swap interval failed");
    }
}
