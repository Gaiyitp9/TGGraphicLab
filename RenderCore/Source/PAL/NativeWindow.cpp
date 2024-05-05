/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "PAL/NativeWindow.h"

namespace TG::PAL
{
    NativeWindow::NativeWindow(int x, int y, int width, int height, wchar_t const *title)
    {
        m_hwnd = CreateWin32Window(x, y, width, height, title);
    }

    inline void PollEvents()
    {
#ifdef _WIN64
        Win32PollEvents();
#endif
    }
}
