/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG
{
    // 窗口基类
    class Window
    {
    public:
        Window(int x, int y, int width, int height, const Window& parent)
            : m_posX(x), m_posY(y), m_width(width), m_height(height), m_parent(parent) {}
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;
        virtual ~Window() = default;

        [[nodiscard]] const Window& Parent() const noexcept { return m_parent; }
        [[nodiscard]] int PositionX() const noexcept { return m_posX; }
        [[nodiscard]] int PositionY() const noexcept { return m_posY; }
        [[nodiscard]] int Width() const noexcept { return m_width; }
        [[nodiscard]] int Height() const noexcept { return m_height; }
        [[nodiscard]] bool Destroy() const noexcept { return m_destroy; }

    protected:
        int m_posX, m_posY;         // 窗口位置
        int m_width, m_height;      // 窗口尺寸
        const Window& m_parent;     // 父母窗口，如果父母窗口销毁，所有子窗口也会销毁
        bool m_destroy = false;     // 是否销毁窗口
    };
}
