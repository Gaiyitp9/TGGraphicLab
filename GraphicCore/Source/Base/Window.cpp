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

    void Window::SetWindowPosCallback(const WindowPosFunction& function)
    {
        m_nativeWindow.windowPosFunction = function;
    }

    void Window::SetWindowSizeCallback(const WindowSizeFunction& function)
    {
        m_nativeWindow.windowSizeFunction = function;
    }

    void Window::SetSuspendCallback(const SuspendFunction& function)
    {
        m_nativeWindow.suspendFunction = function;
    }

    void Window::SetResumeCallback(const ResumeFunction& function)
    {
        m_nativeWindow.resumeFunction = function;
    }

    void Window::SetKeyCallback(const KeyFunction& function)
    {
        m_nativeWindow.keyFunction = function;
    }

    void Window::SetCharCallback(const CharFunction& function)
    {
        m_nativeWindow.charFunction = function;
    }

    void Window::SetMouseButtonCallback(const MouseButtonFunction& function)
    {
        m_nativeWindow.mouseButtonFunction = function;
    }

    void Window::SetCursorPosCallback(const CursorPosFunction& function)
    {
        m_nativeWindow.cursorPosFunction = function;
    }

    void Window::SetScrollCallback(const ScrollFunction& function)
    {
        m_nativeWindow.scrollFunction = function;
    }
}
