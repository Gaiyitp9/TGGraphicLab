/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Base/Core.h"
#include "Base/Windows/Win32API.h"

namespace TG::Rendering
{
    class TG_API Win32OpenGLContext
    {
    public:
        explicit Win32OpenGLContext(HDC hdc);
        ~Win32OpenGLContext();

        Win32OpenGLContext(const Win32OpenGLContext&) = delete;
        Win32OpenGLContext& operator=(const Win32OpenGLContext&) = delete;
        Win32OpenGLContext(Win32OpenGLContext&&) = delete;
        Win32OpenGLContext& operator=(Win32OpenGLContext&&) = delete;

        void MakeCurrent() const;
        void SetVSync(bool enable) const;
        void Present() const;

    private:
        void LoadWGLExtension() const;

        HDC m_hdc{};
        HGLRC m_wglContext{};
    };
}
