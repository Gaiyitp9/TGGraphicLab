/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "WinAPILean.h"
#include <string>

namespace TG::PAL
{
    // 轮询输入事件
    void PollEvents();

    // 窗口分类
    enum class WindowCategory : unsigned char
    {
        Default
    };

    // 获取窗口类对应的WIN32窗口名称
    wchar_t const* GetWindowClassName(const WindowCategory &type);
    // 获取窗口消息
    std::string GetWindowMessageInfo(UINT msg, WPARAM wp, LPARAM lp);

    void CreateNativeWindow();
}
