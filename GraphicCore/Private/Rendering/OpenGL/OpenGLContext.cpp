/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Rendering/OpenGL/OpenGLContext.h"
#include "glad/wgl.h"

namespace TG::Rendering
{
    OpenGLContext::OpenGLContext(const IDefaultVideoPort& videoPort)
    {
        PIXELFORMATDESCRIPTOR pfd{
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            32,
            0, 0, 0, 0, 0, 0, 8, 0,
            0, 0, 0, 0, 0,
            24,
            8,
            0,
            0,
            0, 0, 0, 0
        };
        m_hdc = videoPort.Context();
        int pixelFormat = ChoosePixelFormat(m_hdc, &pfd);
        if (pixelFormat == 0)
            throw BaseException::Create("Failed to get a valid ChoosePixelFormat");
        if (!SetPixelFormat(m_hdc, pixelFormat, &pfd))
            throw BaseException::Create("Failed to set the pixel format");
        // 创建wgl context
        m_wglContext = wglCreateContext(m_hdc);
        if (!m_wglContext)
            throw BaseException::Create("Failed to create a wgl context");

        wglMakeCurrent(m_hdc, m_wglContext);
        // 加载wgl扩展函数，需要创建wgl context才能完成
        gladLoaderLoadWGL(m_hdc);

        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(m_wglContext);
        // 指定OpenGL版本，重新创建wgl context
        int contextAttribList[] =
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 6,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0,
        };
        // 创建wgl context
        m_wglContext = wglCreateContextAttribsARB(m_hdc, nullptr, contextAttribList);
        if (!m_wglContext)
            throw BaseException::Create("Failed to create a wgl context");

        wglMakeCurrent(m_hdc, m_wglContext);
        // 开启垂直同步
        wglSwapIntervalEXT(1);

        // 加载OpenGL函数
        gladLoaderLoadGL();
    }

    OpenGLContext::~OpenGLContext()
    {
        gladLoaderUnloadGL();
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(m_wglContext);
    }

	void OpenGLContext::MakeCurrent() const
	{
    	wglMakeCurrent(m_hdc, m_wglContext);
	}

    void OpenGLContext::SetVSync(bool enable) const
    {
        wglSwapIntervalEXT(enable ? 1 : 0);
    }

    void OpenGLContext::SwapBuffers() const
    {
		::SwapBuffers(m_hdc);
    }
}
