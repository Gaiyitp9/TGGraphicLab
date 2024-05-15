/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "InputEnums.h"
#include <functional>
#include <memory>
#include <optional>

namespace TG::PAL
{
    // 原生窗口内部实现，不同平台的实现不同，比如在Windows平台下内部实现包含Win32窗口句柄
    struct NativeWindow;
    using KeyFunction = std::function<void(KeyCode key, int scanCode, InputAction action, int mods)>;
    using CharFunction = std::function<void(unsigned int c)>;
    using MouseButtonFunction = std::function<void(MouseButton button, InputAction action, int mods)>;
    using CursorPosFunction = std::function<void(int xPos, int yPos)>;
    using ScrollFunction = std::function<void(int xOffset, int yOffset)>;
    using WindowPosFunction = std::function<void(int xPos, int yPos)>;
    using WindowSizeFunction = std::function<void(unsigned int width, unsigned int height)>;
    // 窗口基类，Window.cpp文件放在不同平台对应的目录下
    class Window
    {
    public:
        Window(int x, int y, int width, int height, wchar_t const* title);
        ~Window();

        [[nodiscard]] bool IsDestroyed() const;

        // 窗口消息事件回调
        void SetKeyCallback(const KeyFunction& function) const;
        void SetCharCallback(const CharFunction& function) const;
        void SetMouseButtonCallback(const MouseButtonFunction& function) const;
        void SetCursorPosCallback(const CursorPosFunction& function) const;
        void SetScrollCallback(const ScrollFunction& function) const;
        void SetWindowPosCallback(const WindowPosFunction& function) const;
        void SetWindowSizeCallback(const WindowSizeFunction& function) const;

    private:
        const std::unique_ptr<NativeWindow> m_nativeWindow;
    };

    // 轮询输入事件，需要每帧都调用
    std::optional<int> PollEvents();
}
