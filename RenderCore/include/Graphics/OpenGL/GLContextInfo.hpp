/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "PlatformHeaders.h"

namespace TG::Graphics
{
    struct GLContextInfo
    {
        HWND hwnd{nullptr};
        PIXELFORMATDESCRIPTOR pfd{0};
    };
}