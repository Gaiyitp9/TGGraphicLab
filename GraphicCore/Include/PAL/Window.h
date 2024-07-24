/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Input.h"
#include <functional>
#include <memory>
#include <optional>
#include <string_view>

#ifdef _WIN64
#include "Windows/Win32API.h"
namespace TG::PAL
{
    using NativeDisplay = HDC;
    using NativeWindowHandle = HWND;
}
#endif

namespace TG::PAL
{
    // 窗口类型
    enum class WindowType : unsigned char
    {
        Default,
        Load,
    };
    // 原生窗口内部实现，不同平台的实现不同，比如在Windows平台下内部实现包含Win32窗口句柄
    struct NativeWindow;

    // 窗口基类，Window.cpp文件放在不同平台对应的目录下
    class Window
    {
        using KeyFunction = std::function<void(Key key, int scanCode, InputAction action)>;
        using CharFunction = std::function<void(unsigned int c)>;
        using MouseButtonFunction = std::function<void(MouseButton button, InputAction action)>;
        using CursorPosFunction = std::function<void(int xPos, int yPos)>;
        using ScrollFunction = std::function<void(int xOffset, int yOffset)>;
        using WindowPosFunction = std::function<void(int xPos, int yPos)>;
        using WindowSizeFunction = std::function<void(unsigned int width, unsigned int height)>;
        using SuspendFunction = std::function<void()>;
        using ResumeFunction = std::function<void()>;

    public:
        Window(int x, int y, int width, int height, std::string_view name, WindowType type);
        virtual ~Window() = 0;

        [[nodiscard]] NativeDisplay GetDisplay() const;
        [[nodiscard]] NativeWindowHandle GetWindowHandle() const;
        void SetIcon(std::string_view iconPath) const;
        [[nodiscard]] bool IsDestroyed() const;

    protected:
        // 窗口消息事件回调
        void SetKeyCallback(const KeyFunction& function) const;
        void SetCharCallback(const CharFunction& function) const;
        void SetMouseButtonCallback(const MouseButtonFunction& function) const;
        void SetCursorPosCallback(const CursorPosFunction& function) const;
        void SetScrollCallback(const ScrollFunction& function) const;
        void SetWindowPosCallback(const WindowPosFunction& function) const;
        void SetWindowSizeCallback(const WindowSizeFunction& function) const;
        void SetSuspendCallback(const SuspendFunction& function) const;
        void SetResumeCallback(const ResumeFunction& function) const;

    private:
        const std::unique_ptr<NativeWindow> m_nativeWindow;
    };

    // 轮询输入事件，需要每帧调用
    std::optional<int> PollEvents();
}
