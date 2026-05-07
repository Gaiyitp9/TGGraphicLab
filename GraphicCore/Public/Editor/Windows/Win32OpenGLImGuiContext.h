/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Base/Core.h"
#include "Base/Windows/Win32API.h"

namespace TG::Editor
{
    class TG_API Win32OpenGLImGuiContext
    {
    public:
        explicit Win32OpenGLImGuiContext(HWND hwnd);
        ~Win32OpenGLImGuiContext();

        void NewFrame();
        void Render();
    };
}
