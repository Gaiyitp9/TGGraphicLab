/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Graphics/OpenGL/DeviceContextGL.hpp"

namespace TG::Graphics
{
	DeviceContextGL::DeviceContextGL(const GLContextInfo& info)
	{
        m_hwnd = info.hwnd;
        m_hdc = GetDC(info.hwnd);
        int pixelFormat = ChoosePixelFormat(m_hdc, &info.pfd);
        SetPixelFormat(m_hdc, pixelFormat, &info.pfd);
        m_hglrc = wglCreateContext(m_hdc);
        // 设置当前线程的rendering context，完成后才能对glad进行初始化，否则会报错
        wglMakeCurrent(m_hdc, m_hglrc);
    }

    DeviceContextGL::~DeviceContextGL()
    {
        ReleaseDC(m_hwnd, m_hdc);
        wglDeleteContext(m_hglrc);
    }

    void DeviceContextGL::MakeContextCurrent() const
    {
        wglMakeCurrent(m_hdc, m_hglrc);
    }
}
