/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Base/Core.h"
#include "Base/Windows/Win32API.h"
#include "Base/WindowDelegates.h"
#include <string>
#include <optional>

namespace TG
{
    class TG_API Win32Window
    {
    public:
        Win32Window(int x, int y, unsigned int width, unsigned int height, std::string_view name);
        ~Win32Window();

        [[nodiscard]] HWND Handle() const;
        [[nodiscard]] HDC Surface() const;

        // 轮询输入事件，需要每帧调用
        [[nodiscard]] std::optional<int> PollEvents();

        void Show(bool show) const;

        void SetIcon(std::string_view iconPath) const;

        [[nodiscard]] int PositionX() const noexcept;
        [[nodiscard]] int PositionY() const noexcept;
        void SetPosition(int x, int y);

        [[nodiscard]] unsigned int Width() const noexcept;
        [[nodiscard]] unsigned int Height() const noexcept;
        void SetSize(unsigned int w, unsigned int h);

        KeyDelegate keyDelegate;
        CharDelegate charDelegate;
        MouseButtonDelegate mouseButtonDelegate;
        CursorPosDelegate cursorPosDelegate;
        ScrollDelegate scrollDelegate;
        WindowPosDelegate windowPosDelegate;
        WindowSizeDelegate windowSizeDelegate;
        SuspendDelegate suspendDelegate;
        ResumeDelegate resumeDelegate;

    private:
        std::string m_name;
        HWND m_handle{ nullptr };
        HDC m_deviceContext{ nullptr };
        int m_posX;
        int m_posY;
        unsigned int m_width;
        unsigned int m_height;
    };
}
