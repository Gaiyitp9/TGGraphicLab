/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Base/Window.h"

namespace TG
{
    std::optional<int> Window::PollEvents()
    {
        MSG msg = { nullptr };

        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                return static_cast<int>(msg.wParam);

            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        return std::nullopt;
    }

    void Window::SetIcon(std::string_view iconPath) const
    {
        m_nativeWindow.SetIcon(iconPath);
    }

    void Window::SetPosition(int x, int y)
    {
        m_posX = x;
        m_posY = y;
        m_nativeWindow.SetPosition(x, y);
    }

    void Window::SetSize(int width, int height)
    {
        m_width = width;
        m_height = height;
        m_nativeWindow.SetSize(width, height);
    }

    void Window::Show(bool show) const
    {
        m_nativeWindow.Show(show);
    }
}
