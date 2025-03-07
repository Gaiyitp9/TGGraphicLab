/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Base/Window.h"

namespace TG
{
    std::optional<int> Window::PollEvents()
    {
        return NativeWindow::PollEvents();
    }

    NativeWindowHandle Window::GetHandle() const noexcept
    {
        return m_nativeWindow.handle;
    }

    NativeDeviceContext Window::GetContext() const noexcept
    {
        return m_nativeWindow.deviceContext;
    }

    bool Window::IsDestroyed() const noexcept
    {
        return m_nativeWindow.destroyed;
    }

    void Window::SetIcon(std::string_view iconPath) const
    {
        m_nativeWindow.SetIcon(iconPath);
    }

    void Window::SetPosition(int x, int y)
    {
        m_posX = x;
        m_posY = y;
        m_nativeWindow.SetPosition(x, y);
    }

    void Window::SetSize(int width, int height)
    {
        m_width = width;
        m_height = height;
        m_nativeWindow.SetSize(width, height);
    }

    void Window::Show(bool show) const
    {
        m_nativeWindow.Show(show);
    }

    void Window::SetWindowPosCallback(const WindowPosDelegate& function)
    {
        m_nativeWindow.windowPosDelegate = function;
    }

    void Window::SetWindowSizeCallback(const WindowSizeDelegate& function)
    {
        m_nativeWindow.windowSizeDelegate = function;
    }

    void Window::SetSuspendCallback(const SuspendDelegate& function)
    {
        m_nativeWindow.suspendDelegate = function;
    }

    void Window::SetResumeCallback(const ResumeDelegate& function)
    {
        m_nativeWindow.resumeDelegate = function;
    }

    void Window::SetKeyCallback(const KeyDelegate& function)
    {
        m_nativeWindow.keyDelegate = function;
    }

    void Window::SetCharCallback(const CharDelegate& function)
    {
        m_nativeWindow.charDelegate = function;
    }

    void Window::SetMouseButtonCallback(const MouseButtonDelegate& function)
    {
        m_nativeWindow.mouseButtonDelegate = function;
    }

    void Window::SetCursorPosCallback(const CursorPosDelegate& function)
    {
        m_nativeWindow.cursorPosDelegate = function;
    }

    void Window::SetScrollCallback(const ScrollDelegate& function)
    {
        m_nativeWindow.scrollDelegate = function;
    }
}
