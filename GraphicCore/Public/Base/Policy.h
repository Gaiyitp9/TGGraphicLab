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
    template <typename NativePolicy>
    concept IsValidNativePolicy = requires
    {
        typename NativePolicy::Window;
        typename NativePolicy::WindowHandle;
        typename NativePolicy::DeviceContext;
    };

#ifdef TG_WINDOWS
    struct DefaultNativePolicy
    {
        using Window = Win32Window;
        using WindowHandle = HWND;
        using DeviceContext = HDC;
    };
#endif
}