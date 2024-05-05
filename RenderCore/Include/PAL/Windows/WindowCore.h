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
    // 创建Windows原生窗口
    HWND CreateWin32Window(int x, int y, int width, int height, wchar_t const *title);
    // 轮询输入事件
    void Win32PollEvents();
    // 窗口消息转成字符串
    std::pmr::string WindowMessageToString(UINT msg, WPARAM wp, LPARAM lp);
}
