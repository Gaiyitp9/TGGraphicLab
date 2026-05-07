/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Base/Core.h"
#include "Base/WindowDelegates.h"
#include "xdg-shell-client-protocol.h"

namespace TG
{
    class TG_API WaylandWindow
    {
    public:
        WaylandWindow(int x, int y, unsigned int width, unsigned int height, std::string_view name);
        ~WaylandWindow();

        [[nodiscard]] wl_display* Handle() const;
        [[nodiscard]] wl_surface* Surface() const;

        // 轮询输入事件，需要每帧调用
        [[nodiscard]] std::optional<int> PollEvents() const;

        void Show(bool show) const;

        void SetIcon(std::string_view iconPath) const;

        [[nodiscard]] int PositionX() const noexcept;
        [[nodiscard]] int PositionY() const noexcept;
        void SetPosition(int x, int y);

        [[nodiscard]] unsigned int Width() const noexcept;
        [[nodiscard]] unsigned int Height() const noexcept;
        void SetSize(unsigned int w, unsigned int h);

        KeyDelegate keyDelegate;
        CharDelegate charDelegate;
        MouseButtonDelegate mouseButtonDelegate;
        CursorPosDelegate cursorPosDelegate;
        ScrollDelegate scrollDelegate;
        WindowPosDelegate windowPosDelegate;
        WindowSizeDelegate windowSizeDelegate;
        SuspendDelegate suspendDelegate;
        ResumeDelegate resumeDelegate;

    private:
        std::string m_name;
        int m_posX;
        int m_posY;
        unsigned int m_width;
        unsigned int m_height;

        static void RegistryGlobal(void* data, wl_registry* registry, uint32_t name,
            char const* interface, uint32_t version);
        static void RegistryGlobalRemove(void* data, wl_registry* wl_registry, uint32_t name);

        static void XDGWMBasePing(void* data, xdg_wm_base* xdgWMBase_, uint32_t serial);

        static void XDGSurfaceConfigure(void* data, xdg_surface* xdgSurface, uint32_t serial);

        static void PointerEnter(void* data, wl_pointer* wl_pointer, uint32_t serial, wl_surface* surface,
            wl_fixed_t surface_x, wl_fixed_t surface_y);
        static void PointerLeave(void* data, wl_pointer* wl_pointer, uint32_t serial, wl_surface* surface);
        static void PointerMotion(void* data, wl_pointer* wl_pointer, uint32_t time,
            wl_fixed_t surface_x, wl_fixed_t surface_y);
        static void PointerButton(void* data, wl_pointer* wl_pointer, uint32_t serial, uint32_t time,
            uint32_t button, uint32_t state);
        static void PointerAxis(void* data, wl_pointer* wl_pointer, uint32_t time, uint32_t axis,
            wl_fixed_t value);

        static void KeyboardKeymap(void* data, wl_keyboard* wl_keyboard, uint32_t format, int32_t fd,
            uint32_t size);
        static void KeyboardKey(void* data, wl_keyboard* wl_keyboard, uint32_t serial, uint32_t time,
            uint32_t key, uint32_t state);
        static void KeyboardEnter(void* data, wl_keyboard* wl_keyboard, uint32_t serial, wl_surface* surface,
            wl_array* keys);
        static void KeyboardLeave(void* data, wl_keyboard* wl_keyboard, uint32_t serial, wl_surface* surface);

        static void ToplevelConfigure(void* data, xdg_toplevel* xdg_toplevel, int32_t width, int32_t height,
            wl_array* states);
        static void ToplevelClose(void* data, xdg_toplevel* toplevel);
        static void ToplevelConfigureBounds(void* data, xdg_toplevel* xdg_toplevel, int32_t width, int32_t height);
        static void ToplevelWMCapabilities(void* data, xdg_toplevel* xdg_toplevel, wl_array* capabilities);

        wl_display* m_display{nullptr};
        int m_waylandFd{-1};
        wl_registry* m_registry{nullptr};
        const wl_registry_listener m_registryListener{
            .global = RegistryGlobal,
            .global_remove = RegistryGlobalRemove,
        };
        wl_compositor* m_compositor{nullptr};

        xdg_wm_base* m_xdgWMBase{nullptr};
        xdg_wm_base_listener m_xdgWMBaseListener{
            .ping = XDGWMBasePing,
        };

        wl_seat* m_seat{nullptr};
        wl_pointer* m_pointer{nullptr};
        const wl_pointer_listener m_pointerListener{
            .enter = PointerEnter,
            .leave = PointerLeave,
            .motion = PointerMotion,
            .button = PointerButton,
            .axis = PointerAxis,
        };
        wl_keyboard* m_keyboard{nullptr};
        const wl_keyboard_listener m_keyboardListener{
            .keymap = KeyboardKeymap,
            .enter = KeyboardEnter,
            .leave = KeyboardLeave,
            .key = KeyboardKey,
        };

        wl_surface* m_surface{nullptr};
        xdg_surface* m_xdgSurface{nullptr};
        xdg_surface_listener m_xdgSurfaceListener{
            .configure = XDGSurfaceConfigure,
        };
        xdg_toplevel* m_xdgTopLevel{nullptr};
        const xdg_toplevel_listener m_toplevelListener{
            .configure = ToplevelConfigure,
            .close = ToplevelClose,
            .configure_bounds = ToplevelConfigureBounds,
            .wm_capabilities = ToplevelWMCapabilities,
        };
        bool m_running{true};
    };
}
