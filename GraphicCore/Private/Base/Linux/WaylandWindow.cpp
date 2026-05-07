/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Base/Linux/WaylandWindow.h"
#include "Exception/ErrnoException.h"
#include <cstring>
#include <poll.h>

namespace TG
{
    WaylandWindow::WaylandWindow(int x, int y, unsigned int width, unsigned int height, std::string_view name)
        : m_name{name}, m_posX{x}, m_posY{y}, m_width{width}, m_height{height}
    {
        // 1. 连接到wayland compositor
        m_display = wl_display_connect(nullptr);
        if (!m_display)
            throw ErrnoException::Create("Unable to connect to wayland compositor");
        m_waylandFd = wl_display_get_fd(m_display);

        // 2. 获取registry并绑定全局接口
        // registry是wayland的"服务发现中心"
        // compositor启动时会向registry注册它支持的所有全局对象
        // 客户端连接后，通过registry事件知道有哪些服务可用，然后用wl_registry_bind绑定到自己需要的版本
        m_registry = wl_display_get_registry(m_display);
        wl_registry_add_listener(m_registry, &m_registryListener, this);
        // Wayland是异步的，add_listener只是注册回调，不会立即收到事件
        // roundtrip发送一个特殊请求并等待回复，确保在此期间的所有registry事件都被处理
        wl_display_roundtrip(m_display);
        if (!m_compositor || !m_xdgWMBase)
            throw BaseException::Create("Required Wayland interface not found");

        // 3. 创建surface
        m_surface = wl_compositor_create_surface(m_compositor);
        m_xdgSurface = xdg_wm_base_get_xdg_surface(m_xdgWMBase, m_surface);
        xdg_surface_add_listener(m_xdgSurface, &m_xdgSurfaceListener, nullptr);

        // 4. 创建窗口
        m_xdgTopLevel = xdg_surface_get_toplevel(m_xdgSurface);
        xdg_toplevel_add_listener(m_xdgTopLevel, &m_toplevelListener, this);
        xdg_toplevel_set_title(m_xdgTopLevel, m_name.c_str());
        xdg_toplevel_set_app_id(m_xdgTopLevel, "TGGraphicLab");

        wl_surface_commit(m_surface);
        wl_display_roundtrip(m_display);
    }

    WaylandWindow::~WaylandWindow()
    {
        xdg_toplevel_destroy(m_xdgTopLevel);
        xdg_surface_destroy(m_xdgSurface);
        wl_surface_destroy(m_surface);

        wl_keyboard_destroy(m_keyboard);
        wl_pointer_destroy(m_pointer);
        wl_seat_destroy(m_seat);
        xdg_wm_base_destroy(m_xdgWMBase);

        wl_compositor_destroy(m_compositor);
        wl_registry_destroy(m_registry);
        wl_display_disconnect(m_display);
        m_waylandFd = -1;
    }

    wl_display* WaylandWindow::Handle() const
    {
        return m_display;
    }

    wl_surface* WaylandWindow::Surface() const
    {
        return m_surface;
    }

    std::optional<int> WaylandWindow::PollEvents() const
    {
        wl_display_dispatch_pending(m_display);

        wl_display_flush(m_display);

        pollfd fd{
            .fd = m_waylandFd,
            .events =  POLLIN,
        };
        const int ret = poll(&fd, 1, 0);
        if (ret < 0)
            throw ErrnoException::Create("Wayland window poll events failed");
        if (ret > 0 && (fd.revents & POLLIN))
            wl_display_dispatch(m_display);

        if (!m_running)
            return -1;

        return std::nullopt;
    }

    void WaylandWindow::SetIcon(std::string_view iconPath) const
    {

    }

    int WaylandWindow::PositionX() const noexcept
    {
        return m_posX;
    }

    int WaylandWindow::PositionY() const noexcept
    {
        return m_posY;
    }

    void WaylandWindow::SetPosition(int x, int y)
    {
        m_posX = x;
        m_posY = y;
    }

    unsigned int WaylandWindow::Width() const noexcept
    {
        return m_width;
    }

    unsigned int WaylandWindow::Height() const noexcept
    {
        return m_height;
    }

    void WaylandWindow::SetSize(unsigned int w, unsigned int h)
    {
        m_width = w;
        m_height = h;
    }

    void WaylandWindow::Show(bool show) const
    {

    }

    void WaylandWindow::RegistryGlobal(void* data, wl_registry* registry, uint32_t name,
                                       char const* interface, uint32_t version)
    {
        auto* waylandWindow = static_cast<WaylandWindow*>(data);
        if (std::strcmp(interface, wl_compositor_interface.name) == 0)
        {
            waylandWindow->m_compositor = static_cast<wl_compositor*>(
                wl_registry_bind(registry, name, &wl_compositor_interface, version));
        }
        else if (std::strcmp(interface, xdg_wm_base_interface.name) == 0)
        {
            waylandWindow->m_xdgWMBase = static_cast<xdg_wm_base*>(
                wl_registry_bind(registry, name, &xdg_wm_base_interface, version));
            xdg_wm_base_add_listener(waylandWindow->m_xdgWMBase, &waylandWindow->m_xdgWMBaseListener, nullptr);
        }
        else if (std::strcmp(interface, wl_seat_interface.name) == 0)
        {
            waylandWindow->m_seat = static_cast<wl_seat*>(
                wl_registry_bind(registry, name, &wl_seat_interface, version));

            waylandWindow->m_pointer = wl_seat_get_pointer(waylandWindow->m_seat);
            wl_pointer_add_listener(waylandWindow->m_pointer, &waylandWindow->m_pointerListener, nullptr);

            waylandWindow->m_keyboard = wl_seat_get_keyboard(waylandWindow->m_seat);
            wl_keyboard_add_listener(waylandWindow->m_keyboard, &waylandWindow->m_keyboardListener, nullptr);
        }
    }

    void WaylandWindow::RegistryGlobalRemove(void*, wl_registry*, uint32_t)
    {

    }

    void WaylandWindow::XDGWMBasePing(void*, xdg_wm_base* xdgWMBase, uint32_t serial)
    {
        xdg_wm_base_pong(xdgWMBase, serial);
    }

    void WaylandWindow::XDGSurfaceConfigure(void*, xdg_surface* xdgSurface, uint32_t serial)
    {
        xdg_surface_ack_configure(xdgSurface, serial);
    }

    void WaylandWindow::PointerEnter(void* data, wl_pointer* wl_pointer, uint32_t serial, wl_surface* surface, wl_fixed_t surface_x, wl_fixed_t surface_y)
    {

    }

    void WaylandWindow::PointerLeave(void* data, wl_pointer* wl_pointer, uint32_t serial, wl_surface* surface)
    {

    }

    void WaylandWindow::PointerMotion(void* data, wl_pointer* wl_pointer, uint32_t time, wl_fixed_t surface_x, wl_fixed_t surface_y)
    {

    }

    void WaylandWindow::PointerButton(void* data, wl_pointer* wl_pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state)
    {

    }

    void WaylandWindow::PointerAxis(void* data, wl_pointer* wl_pointer, uint32_t time, uint32_t axis, wl_fixed_t value)
    {

    }

    void WaylandWindow::KeyboardKeymap(void* data, wl_keyboard* wl_keyboard, uint32_t format, int32_t fd, uint32_t size)
    {

    }

    void WaylandWindow::KeyboardEnter(void* data, wl_keyboard* wl_keyboard, uint32_t serial, wl_surface* surface, wl_array* keys)
    {

    }

    void WaylandWindow::KeyboardLeave(void* data, wl_keyboard* wl_keyboard, uint32_t serial, wl_surface* surface)
    {

    }

    void WaylandWindow::KeyboardKey(void* data, wl_keyboard* wl_keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state)
    {

    }

    void WaylandWindow::ToplevelConfigure(void* data, xdg_toplevel* xdg_toplevel, int32_t width,
        int32_t height, wl_array* states)
    {

    }

    void WaylandWindow::ToplevelClose(void* data, xdg_toplevel* toplevel)
    {
        auto* waylandWindow = static_cast<WaylandWindow*>(data);
        waylandWindow->m_running = false;
    }

    void WaylandWindow::ToplevelConfigureBounds(void* data, xdg_toplevel* xdg_toplevel, int32_t width, int32_t height)
    {

    }

    void WaylandWindow::ToplevelWMCapabilities(void* data, xdg_toplevel* xdg_toplevel, wl_array* capabilities)
    {

    }
}
