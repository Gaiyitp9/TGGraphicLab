/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Win32API.h"
#include "PAL/InputEnums.h"
#include <string>

namespace TG::PAL
{
    using KeyFunction = std::function<void(KeyCode key, int scanCode, InputAction action, int mods)>;
    using CharFunction = std::function<void(unsigned int c)>;
    using MouseButtonFunction = std::function<void(MouseButton button, InputAction action, int mods)>;
    using CursorPosFunction = std::function<void(int xPos, int yPos)>;
    using ScrollFunction = std::function<void(int xOffset, int yOffset)>;
    using WindowPosFunction = std::function<void(int xPos, int yPos)>;
    using WindowSizeFunction = std::function<void(unsigned int width, unsigned int height)>;
    // Windows原生窗口
    struct NativeWindow
    {
        std::string name{};
        HWND        hwnd{nullptr};
        bool        spyMessage{false};
        bool        destroyed{false};
        KeyFunction         keyFunction{};
        CharFunction        charFunction{};
        MouseButtonFunction mouseButtonFunction{};
        CursorPosFunction   cursorPosFunction{};
        ScrollFunction      scrollFunction{};
        WindowPosFunction   windowPosFunction{};
        WindowSizeFunction  windowSizeFunction{};
    };
}
