/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Win32API.h"
#include <string>

namespace TG::PAL
{
    // Windows原生窗口
    struct NativeWindow
    {
        std::string name{};
        HWND hwnd{nullptr};
        bool spyMessage{false};
        bool destroyed{false};
        std::function<void(int key, int scancode, int action, int mods)> keyFunction{};
        std::function<void(unsigned int c)> charFunction{};
        std::function<void(int button, int action, int mods)> mouseButtonFunction{};
    };
}
