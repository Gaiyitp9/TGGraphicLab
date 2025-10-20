/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Base/Delegate.hpp"
#include "Input/KeyCode.h"

namespace TG
{
    // 窗口类型
    enum class WindowType : unsigned char
    {
        Default,
        Main,
        Load,
    };
    // 窗口消息委托类型
    using KeyDelegate = Delegate<void(Input::KeyCode key, int scanCode, Input::Action action)>;
    using CharDelegate = Delegate<void(char16_t c)>;
    using MouseButtonDelegate = Delegate<void(Input::KeyCode button, Input::Action action)>;
    using CursorPosDelegate = Delegate<void(int xPos, int yPos)>;
    using ScrollDelegate = Delegate<void(int xOffset, int yOffset)>;
    using WindowPosDelegate = Delegate<void(int xPos, int yPos)>;
    using WindowSizeDelegate = Delegate<void(unsigned int width, unsigned int height)>;
    using SuspendDelegate = Delegate<void()>;
    using ResumeDelegate = Delegate<void()>;
}
