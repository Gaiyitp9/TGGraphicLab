/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Base/Core.h"
#include "Input/KeyCode.h"
#include <string>
#include <optional>
#include "Base/Delegate.hpp"

namespace TG
{
    // 窗口类型
    enum class WindowType : unsigned char
    {
        Default,
        Load,
    };

    using NativeDeviceContext = HDC;
    using NativeWindowHandle = HWND;

    using KeyDelegate = Delegate<void(Input::KeyCode key, int scanCode, Input::Action action)>;
    using CharDelegate = Delegate<void(char16_t c)>;
    using MouseButtonDelegate = Delegate<void(Input::KeyCode button, Input::Action action)>;
    using CursorPosDelegate = Delegate<void(int xPos, int yPos)>;
    using ScrollDelegate = Delegate<void(int xOffset, int yOffset)>;
    using WindowPosDelegate = Delegate<void(int xPos, int yPos)>;
    using WindowSizeDelegate = Delegate<void(unsigned int width, unsigned int height)>;
    using SuspendDelegate = Delegate<void()>;
    using ResumeDelegate = Delegate<void()>;

    // 封装Win32原生窗口
    struct NativeWindow
    {
        NativeWindow(int x, int y, int width, int height, std::string_view name, WindowType type);
        ~NativeWindow();

        // 轮询输入事件，需要每帧调用
        static std::optional<int> PollEvents();

        void SetIcon(std::string_view iconPath) const;
        void SetPosition(int x, int y) const;
        void SetSize(int width, int height) const;
        void Show(bool show) const;

        std::string name;
        HWND    handle{ nullptr };
        HDC     deviceContext{ nullptr };
        bool    destroyed{ false };

        KeyDelegate keyDelegate;
        CharDelegate charDelegate{};
        MouseButtonDelegate mouseButtonDelegate;
        CursorPosDelegate cursorPosDelegate;
        ScrollDelegate scrollDelegate;
        WindowPosDelegate windowPosDelegate;
        WindowSizeDelegate windowSizeDelegate;
        SuspendDelegate suspendDelegate;
        ResumeDelegate resumeDelegate;
    };
}
