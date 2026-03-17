/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Rendering/OpenGL/OpenGLContext.h"
#include "Exception/Windows/Win32Exception.h"
#include "glad/wgl.h"

namespace TG::Rendering
{
    OpenGLContext::OpenGLContext(const IDefaultVideoPort& videoPort)
	    : m_videoPort{ videoPort }, m_hdc{ videoPort.Context() }
    {
    	LoadWGLExtension();

    	// 设置缓冲格式
    	int pixelAttribs[] =
    	{
    		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
			WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB,     32,
			WGL_DEPTH_BITS_ARB,     24,
			WGL_STENCIL_BITS_ARB,   8,
    		WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,
			0
		};

    	int pixelFormat;
    	UINT numFormats;
    	wglChoosePixelFormatARB(
			m_hdc,
			pixelAttribs,
			nullptr,
			1,
			&pixelFormat,
			&numFormats
		);

    	if (!SetPixelFormat(m_hdc, pixelFormat, nullptr))
    		throw Win32Exception::Create("Failed to set the pixel format");
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

    	// 启用sRGB缓冲
    	glEnable(GL_FRAMEBUFFER_SRGB);
    }

    OpenGLContext::~OpenGLContext()
    {
        gladLoaderUnloadGL();
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(m_wglContext);
    }

	const IDefaultVideoPort& OpenGLContext::VideoPort() const
    {
	    return m_videoPort;
    }

	void OpenGLContext::MakeCurrent() const
	{
    	wglMakeCurrent(m_hdc, m_wglContext);
	}

    void OpenGLContext::SetVSync(bool enable) const
    {
        wglSwapIntervalEXT(enable ? 1 : 0);
    }

    void OpenGLContext::Present() const
    {
		::SwapBuffers(m_hdc);
    }

    void OpenGLContext::LoadWGLExtension() const
    {
    	WNDCLASS wc{};
    	wc.style = CS_OWNDC;
    	wc.lpfnWndProc = DefWindowProc;
    	wc.hInstance = GetModuleHandle(nullptr);
    	wc.lpszClassName = "Dummy_WGL";

    	RegisterClass(&wc);

    	HWND hwnd = CreateWindow(
			wc.lpszClassName, "",
			WS_OVERLAPPEDWINDOW,
			0, 0, 1, 1,
			nullptr, nullptr,
			wc.hInstance, nullptr);

    	HDC hdc = GetDC(hwnd);

    	PIXELFORMATDESCRIPTOR pfd{};
    	pfd.nSize = sizeof(pfd);
    	pfd.nVersion = 1;
    	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    	pfd.iPixelType = PFD_TYPE_RGBA;
    	pfd.cColorBits = 32;

    	int format = ChoosePixelFormat(hdc, &pfd);
    	SetPixelFormat(hdc, format, &pfd);

    	HGLRC rc = wglCreateContext(hdc);
    	wglMakeCurrent(hdc, rc);

    	// 加载wgl扩展函数，需要创建wgl context才能完成
    	gladLoaderLoadWGL(hdc);

    	wglMakeCurrent(nullptr, nullptr);
    	wglDeleteContext(rc);
    	ReleaseDC(hwnd, hdc);
    	DestroyWindow(hwnd);
    	UnregisterClass("Dummy_WGL", wc.hInstance);
    }
}
