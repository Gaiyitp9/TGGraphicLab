/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Policy.h"

namespace TG
{
    // 显示接口
    template <typename NativePolicy>
    struct IVideoPort
    {
        virtual ~IVideoPort() = default;

        [[nodiscard]] virtual NativePolicy::WindowHandle GetHandle() const noexcept = 0;
        [[nodiscard]] virtual NativePolicy::DeviceContext GetContext() const noexcept = 0;
        [[nodiscard]] virtual int Width() const noexcept = 0;
        [[nodiscard]] virtual int Height() const noexcept = 0;
    };

    // 窗口基类
    template <typename NativePolicy> requires IsValidNativePolicy<NativePolicy>
    class Window : NativePolicy::Window, public IVideoPort<NativePolicy>
    {
        using Super = NativePolicy::Window;

    public:
        Window(int x, int y, int width, int height, std::string_view name, WindowType type)
            : Super(x, y, width, height, name, type),
              m_posX(x), m_posY(y), m_width(width), m_height(height)
        {}
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;
        ~Window() override = default;

        // 轮询输入事件，需要每帧调用
        static std::optional<int> PollEvents()
        {
            return Super::PollEvents();
        }

        [[nodiscard]] NativePolicy::WindowHandle GetHandle() const noexcept override
        {
            return Super::GetHandle();
        }
        [[nodiscard]] NativePolicy::DeviceContext GetContext() const noexcept override
        {
            return Super::GetContext();
        }
        [[nodiscard]] bool IsDestroyed() const noexcept
        {
            return Super::IsDestroyed();
        }
        [[nodiscard]] int PositionX() const noexcept { return m_posX; }
        [[nodiscard]] int PositionY() const noexcept { return m_posY; }
        [[nodiscard]] int Width() const noexcept override { return m_width; }
        [[nodiscard]] int Height() const noexcept override { return m_height; }

        void SetIcon(std::string_view iconPath) const
        {
            Super::SetIcon(iconPath);
        }
        void SetPosition(int x, int y)
        {
            m_posX = x;
            m_posY = y;
            Super::SetPosition(x, y);
        }
        void SetSize(unsigned int width, unsigned int height)
        {
            m_width = width;
            m_height = height;
            Super::SetSize(width, height);
        }
        void Show(bool show) const
        {
            Super::Show(show);
        }

        // 窗口消息事件回调
        void SetWindowPosCallback(const WindowPosDelegate& delegate)
        {
            Super::SetWindowPosCallback(delegate);
        }
        void SetWindowSizeCallback(const WindowSizeDelegate& delegate)
        {
            Super::SetWindowSizeCallback(delegate);
        }
        void SetSuspendCallback(const SuspendDelegate& delegate)
        {
            Super::SetSuspendCallback(delegate);
        }
        void SetResumeCallback(const ResumeDelegate& delegate)
        {
            Super::SetResumeCallback(delegate);
        }
        void SetKeyCallback(const KeyDelegate& delegate)
        {
            Super::SetKeyCallback(delegate);
        }
        void SetCharCallback(const CharDelegate& delegate)
        {
            Super::SetCharCallback(delegate);
        }
        void SetMouseButtonCallback(const MouseButtonDelegate& delegate)
        {
            Super::SetMouseButtonCallback(delegate);
        }
        void SetCursorPosCallback(const CursorPosDelegate& delegate)
        {
            Super::SetCursorPosCallback(delegate);
        }
        void SetScrollCallback(const ScrollDelegate& delegate)
        {
            Super::SetScrollCallback(delegate);
        }

    private:
        int m_posX;
        int m_posY;
        unsigned int m_width;
        unsigned int m_height;
    };

    using WindowBase = Window<DefaultNativePolicy>;
    using IDefaultVideoPort = IVideoPort<DefaultNativePolicy>;
}
