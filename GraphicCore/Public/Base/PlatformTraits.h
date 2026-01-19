/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#ifdef TG_WINDOWS
    #include "Base/Windows/Win32Window.h"
#endif

namespace TG
{
    template <typename PlatformTraits>
    concept IsValidPlatformTraits = requires
    {
        typename PlatformTraits::Window;
        typename PlatformTraits::WindowHandle;
        typename PlatformTraits::DeviceContext;
    };

#ifdef TG_WINDOWS
    struct DefaultPlatformTraits
    {
        using Window = Win32Window;
        using WindowHandle = HWND;
        using DeviceContext = HDC;
    };
#endif
}
