/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Base/Core.h"
#include "Base/WindowDeclarations.h"
#include <string>
#include <optional>

namespace TG
{
    // 封装Win32原生窗口
    class Win32Window
    {
    public:
        Win32Window(int x, int y, int width, int height, std::string_view name, WindowType type);
        virtual ~Win32Window();

        // 轮询输入事件，需要每帧调用
        static std::optional<int> PollEvents();

        [[nodiscard]] HWND GetHandle() const noexcept { return m_handle; }
        [[nodiscard]] HDC GetContext() const noexcept { return m_deviceContext; }
        [[nodiscard]] bool IsDestroyed() const noexcept { return m_destroyed; }

        void SetIcon(std::string_view iconPath) const;
        void SetPosition(int x, int y) const;
        void SetSize(unsigned int width, unsigned int height) const;
        void Show(bool show) const;

        void SetWindowPosCallback(const WindowPosDelegate& delegate)
        {
            m_windowPosDelegate = delegate;
        }
        void SetWindowSizeCallback(const WindowSizeDelegate& delegate)
        {
            m_windowSizeDelegate = delegate;
        }
        void SetSuspendCallback(const SuspendDelegate& delegate)
        {
            m_suspendDelegate = delegate;
        }
        void SetResumeCallback(const ResumeDelegate& delegate)
        {
            m_resumeDelegate = delegate;
        }
        void SetKeyCallback(const KeyDelegate& delegate)
        {
            m_keyDelegate = delegate;
        }
        void SetCharCallback(const CharDelegate& delegate)
        {
            m_charDelegate = delegate;
        }
        void SetMouseButtonCallback(const MouseButtonDelegate& delegate)
        {
            m_mouseButtonDelegate = delegate;
        }
        void SetCursorPosCallback(const CursorPosDelegate& delegate)
        {
            m_cursorPosDelegate = delegate;
        }
        void SetScrollCallback(const ScrollDelegate& delegate)
        {
            m_scrollDelegate = delegate;
        }

    private:
        std::string m_name;
        HWND    m_handle{ nullptr };
        HDC     m_deviceContext{ nullptr };
        bool    m_destroyed{ false };

        KeyDelegate m_keyDelegate;
        CharDelegate m_charDelegate{};
        MouseButtonDelegate m_mouseButtonDelegate;
        CursorPosDelegate m_cursorPosDelegate;
        ScrollDelegate m_scrollDelegate;
        WindowPosDelegate m_windowPosDelegate;
        WindowSizeDelegate m_windowSizeDelegate;
        SuspendDelegate m_suspendDelegate;
        ResumeDelegate m_resumeDelegate;

        friend LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    };
}
