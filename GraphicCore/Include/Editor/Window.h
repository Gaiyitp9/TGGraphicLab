/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "PAL/Window.h"

namespace TG
{
    // 窗口基类
    class Window : public PAL::Window
    {
    public:
        Window(int x, int y, int width, int height, std::string_view name, PAL::WindowType type)
            : PAL::Window(x, y, width, height, name, type), m_posX(x), m_posY(y), m_width(width),
                m_height(height)
        {}
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;
        ~Window() override = 0;

        [[nodiscard]] int PositionX() const noexcept { return m_posX; }
        [[nodiscard]] int PositionY() const noexcept { return m_posY; }
        [[nodiscard]] int Width() const noexcept { return m_width; }
        [[nodiscard]] int Height() const noexcept { return m_height; }

    protected:
        int m_posX, m_posY;     // 窗口位置
        int m_width, m_height;  // 窗口尺寸
    };
}
