/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Base/Core.h"

namespace TG::Editor
{
    class TG_API LinuxOpenGLImGuiContext
    {
    public:
        LinuxOpenGLImGuiContext();
        ~LinuxOpenGLImGuiContext();

        void NewFrame();
        void Render();
    };
}
