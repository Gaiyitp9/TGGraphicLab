/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#ifdef TG_WINDOWS
    #include "Base/Windows/Win32Window.h"
    #include "Rendering/OpenGL/Windows/Win32OpenGLContext.h"
    #include "Editor/Windows/Win32OpenGLImGuiContext.h"
#endif

#ifdef TG_LINUX
    #include "Base/Linux/WaylandWindow.h"
    #include "Rendering/OpenGL/Linux/LinuxOpenGLContext.h"
    #include "Editor/Linux/LinuxOpenGLImGuiContext.h"
#endif

namespace TG
{
    enum class Platform
    {
        Windows,
        Linux,
    };

    template<Platform P> struct PlatformTraits;

#ifdef TG_WINDOWS
    template<>
    struct PlatformTraits<Platform::Windows>
    {
        using Window = Win32Window;
        using WindowHandle = HWND;
        using RenderSurface = HDC;
        using OpenGLContext = Rendering::Win32OpenGLContext;
        using ImGuiOpenGLContext = Editor::Win32OpenGLImGuiContext;
    };

    inline constexpr auto g_platform = Platform::Windows;
#endif

#ifdef TG_LINUX
    template<>
    struct PlatformTraits<Platform::Linux>
    {
        using Window = WaylandWindow;
        using WindowHandle = wl_display*;
        using RenderSurface = wl_surface*;
        using OpenGLContext = Rendering::LinuxOpenGLContext;
        using ImGuiOpenGLContext = Editor::LinuxOpenGLImGuiContext;
    };

    inline constexpr auto g_platform = Platform::Linux;
#endif
}
