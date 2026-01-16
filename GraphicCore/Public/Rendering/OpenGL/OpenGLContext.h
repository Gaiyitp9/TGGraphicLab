/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Base/CommonInterfaces.h"

namespace TG::Rendering
{
    class OpenGLContext
    {
    public:
        explicit OpenGLContext(const IDefaultVideoPort& videoPort);
        ~OpenGLContext();

        void MakeCurrent() const;
        void SetVSync(bool enable) const;
        void SwapBuffers() const;

    private:
        HDC m_hdc{};
        HGLRC m_wglContext{};
    };
}
