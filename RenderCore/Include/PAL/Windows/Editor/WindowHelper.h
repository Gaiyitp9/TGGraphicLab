/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "../Win32Lean.h"
#include <string>

namespace TG
{
    // 窗口种类
    enum class WindowType : unsigned char
    {
        Default
    };
    // 获取窗口类的名称
    wchar_t const* GetWindowClassName(const WindowType &type);
    // 获取窗口消息
    std::string GetWindowMessageInfo(UINT msg, WPARAM wp, LPARAM lp);
}
