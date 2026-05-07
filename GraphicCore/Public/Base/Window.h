/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Base/Interfaces.h"

namespace TG
{
    // 窗口基类
    class TG_API Window : public IVideoPort
    {
        using PlatformTraitsT = PlatformTraits<g_platform>;

    public:
        Window(int x, int y, unsigned int width, unsigned int height, std::string_view name)
            : m_window{x, y, width, height, name}
        {}
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;
        ~Window() override = default;

        [[nodiscard]] PlatformTraitsT::WindowHandle Handle() const noexcept override
        {
            return m_window.Handle();
        }
        [[nodiscard]] PlatformTraitsT::RenderSurface Surface() const noexcept override
        {
            return m_window.Surface();
        }

        void Show(bool show) const { m_window.Show(show); }

        void SetIcon(std::string_view iconPath) const { m_window.SetIcon(iconPath); }

        [[nodiscard]] int PositionX() const noexcept { return m_window.PositionX(); }
        [[nodiscard]] int PositionY() const noexcept { return m_window.PositionY(); }
        void SetPosition(int x, int y) { m_window.SetPosition(x, y); }

        [[nodiscard]] unsigned int Width() const noexcept override { return m_window.Width(); }
        [[nodiscard]] unsigned int Height() const noexcept override { return m_window.Height(); }
        void SetSize(unsigned int width, unsigned int height) { m_window.SetSize(width, height); }

        // 轮询输入事件，需要每帧调用
        [[nodiscard]] std::optional<int> PollEvents() const
        {
            return m_window.PollEvents();
        }

        // 窗口消息事件回调
        void SetWindowPosCallback(const WindowPosDelegate& delegate)
        {
            m_window.windowPosDelegate = delegate;
        }
        void SetWindowSizeCallback(const WindowSizeDelegate& delegate)
        {
            m_window.windowSizeDelegate = delegate;
        }
        void SetSuspendCallback(const SuspendDelegate& delegate)
        {
            m_window.suspendDelegate = delegate;
        }
        void SetResumeCallback(const ResumeDelegate& delegate)
        {
            m_window.resumeDelegate = delegate;
        }
        void SetKeyCallback(const KeyDelegate& delegate)
        {
            m_window.keyDelegate = delegate;
        }
        void SetCharCallback(const CharDelegate& delegate)
        {
            m_window.charDelegate = delegate;
        }
        void SetMouseButtonCallback(const MouseButtonDelegate& delegate)
        {
            m_window.mouseButtonDelegate = delegate;
        }
        void SetCursorPosCallback(const CursorPosDelegate& delegate)
        {
            m_window.cursorPosDelegate = delegate;
        }
        void SetScrollCallback(const ScrollDelegate& delegate)
        {
            m_window.scrollDelegate = delegate;
        }
        
    private:
        PlatformTraitsT::Window m_window;
    };
}
