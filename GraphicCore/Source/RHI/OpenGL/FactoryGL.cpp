/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Graphics/OpenGL/FactoryGL.hpp"

namespace TG::Graphics
{
    FactoryGL::~FactoryGL()
    {
        if (m_gladInit)
            gladLoaderUnloadGL();
    }

    void FactoryGL::CreateDeviceAndContext(const GLCreateInfo& info, RenderDeviceGL** ppDevice, DeviceContextGL** ppContext)
    {
        if (info.hwnd == nullptr)
            throw BaseException(L"hwnd can't be nullptr for creating device and context.");

        GLContextInfo ctxInfo;
        ctxInfo.hwnd = info.hwnd;
        ctxInfo.pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        ctxInfo.pfd.nVersion = 1;
        ctxInfo.pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        ctxInfo.pfd.iPixelType = PFD_TYPE_RGBA;
        ctxInfo.pfd.cColorBits = 24;
        ctxInfo.pfd.cDepthBits = 24;
        ctxInfo.pfd.cStencilBits = 8;

        *ppDevice = TG_NEW RenderDeviceGL();
        *ppContext = TG_NEW DeviceContextGL(ctxInfo);

        if (!m_gladInit)
        {
            gladLoaderLoadGL();
            gladLoaderLoadWGL((HDC)INVALID_HANDLE_VALUE);
            m_gladInit = true;
        }
    }
}