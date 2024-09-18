/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Platform/Core.h"
#include "PAL/Input.h"
#include <string>

namespace TG::PAL
{
    // Windows原生窗口
    struct NativeWindow
    {
        std::string name{};
        HWND        hwnd{ nullptr };
        bool        spyMessage{ false };
        bool        destroyed{ false };
        std::function<void(Key key, int scanCode, InputAction action)> keyFunction{};
        std::function<void(unsigned int c)> charFunction{};
        std::function<void(MouseButton button, InputAction action)> mouseButtonFunction{};
        std::function<void(int xPos, int yPos)> cursorPosFunction{};
        std::function<void(int xOffset, int yOffset)> scrollFunction{};
        std::function<void(int xPos, int yPos)> windowPosFunction{};
        std::function<void(unsigned int width, unsigned int height)> windowSizeFunction{};
        std::function<void()> suspendFunction{};
        std::function<void()> resumeFunction{};
    };
}
