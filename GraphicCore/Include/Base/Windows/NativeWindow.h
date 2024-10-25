/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Base/Core.h"
#include "Input/KeyCode.h"
#include <string>
#include <functional>

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

    using KeyFunction = std::function<void(Input::KeyCode key, int scanCode, Input::Action action)>;
    using CharFunction = std::function<void(char16_t c)>;
    using MouseButtonFunction = std::function<void(Input::KeyCode button, Input::Action action)>;
    using CursorPosFunction = std::function<void(int xPos, int yPos)>;
    using ScrollFunction = std::function<void(int xOffset, int yOffset)>;
    using WindowPosFunction = std::function<void(int xPos, int yPos)>;
    using WindowSizeFunction = std::function<void(unsigned int width, unsigned int height)>;
    using SuspendFunction = std::function<void()>;
    using ResumeFunction = std::function<void()>;

    // 封装Win32原生窗口
    struct NativeWindow
    {
        NativeWindow(int x, int y, int width, int height, std::string_view name, WindowType type);
        ~NativeWindow();

        void SetIcon(std::string_view iconPath) const;
        void SetPosition(int x, int y) const;
        void SetSize(int width, int height) const;
        void Show(bool show) const;

        std::string name;
        HWND    handle{ nullptr };
        HDC     display{ nullptr };
        bool    destroyed{ false };

        std::function<void(Input::KeyCode key, int scanCode, Input::Action action)> keyFunction;
        std::function<void(char16_t c)> charFunction{};
        std::function<void(Input::KeyCode button, Input::Action action)> mouseButtonFunction;
        std::function<void(int xPos, int yPos)> cursorPosFunction;
        std::function<void(int xOffset, int yOffset)> scrollFunction;
        std::function<void(int xPos, int yPos)> windowPosFunction;
        std::function<void(unsigned int width, unsigned int height)> windowSizeFunction;
        std::function<void()> suspendFunction;
        std::function<void()> resumeFunction;
    };
}
