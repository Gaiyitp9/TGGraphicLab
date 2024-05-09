/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Win32API.h"
#include <functional>
#include <string>

namespace TG::PAL
{
    // Windows原生窗口
    class NativeWindow
    {
    public:
        NativeWindow(int x, int y, int width, int height, wchar_t const* title);

    private:
        LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

        HWND m_hwnd;
    };
}
