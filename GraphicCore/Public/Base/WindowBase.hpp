/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "CommonInterfaces.h"

namespace TG
{
    // 窗口基类
    template <typename PlatformTraits>
    class Window : PlatformTraits::Window, public IVideoPort<PlatformTraits>
    {
        using Super = PlatformTraits::Window;

    public:
        Window(int x, int y, unsigned int width, unsigned int height, std::string_view name, WindowType type)
            : Super(x, y, width, height, name, type)
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

        [[nodiscard]] int PositionX() const noexcept { return Super::posX; }
        [[nodiscard]] int PositionY() const noexcept { return Super::posY; }
        [[nodiscard]] int Width() const noexcept override { return Super::width; }
        [[nodiscard]] int Height() const noexcept override { return Super::height; }

        [[nodiscard]] PlatformTraits::WindowHandle Handle() const noexcept override
        {
            return Super::handle;
        }
        [[nodiscard]] PlatformTraits::DeviceContext Context() const noexcept override
        {
            return Super::deviceContext;
        }
        [[nodiscard]] bool IsDestroyed() const noexcept
        {
            return Super::destroyed;
        }

        void SetIcon(std::string_view iconPath) const
        {
            Super::SetIcon(iconPath);
        }
        void SetPosition(int x, int y)
        {
            Super::SetPosition(x, y);
        }
        void SetSize(unsigned int width, unsigned int height)
        {
            Super::SetSize(width, height);
        }
        void Show(bool show) const
        {
            Super::Show(show);
        }

        // 窗口消息事件回调
        void SetWindowPosCallback(const WindowPosDelegate& delegate)
        {
            Super::windowPosDelegate = delegate;
        }
        void SetWindowSizeCallback(const WindowSizeDelegate& delegate)
        {
            Super::windowSizeDelegate = delegate;
        }
        void SetSuspendCallback(const SuspendDelegate& delegate)
        {
            Super::suspendDelegate = delegate;
        }
        void SetResumeCallback(const ResumeDelegate& delegate)
        {
            Super::resumeDelegate = delegate;
        }
        void SetKeyCallback(const KeyDelegate& delegate)
        {
            Super::keyDelegate = delegate;
        }
        void SetCharCallback(const CharDelegate& delegate)
        {
            Super::charDelegate = delegate;
        }
        void SetMouseButtonCallback(const MouseButtonDelegate& delegate)
        {
            Super::mouseButtonDelegate = delegate;
        }
        void SetCursorPosCallback(const CursorPosDelegate& delegate)
        {
            Super::cursorPosDelegate = delegate;
        }
        void SetScrollCallback(const ScrollDelegate& delegate)
        {
            Super::scrollDelegate = delegate;
        }
    };

    using WindowBase = Window<DefaultPlatformTraits>;
}
