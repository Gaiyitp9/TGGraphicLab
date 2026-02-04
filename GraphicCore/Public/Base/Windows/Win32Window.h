/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Base/Core.h"
#include "Base/WindowDeclarations.h"
#include <string>
#include <optional>

namespace TG
{
    // 封装Win32原生窗口
    struct Win32Window
    {
        Win32Window(int x, int y, unsigned int width, unsigned int height, std::string_view name, WindowType type);
        virtual ~Win32Window();

        // 轮询输入事件，需要每帧调用
        static std::optional<int> PollEvents();

        void SetIcon(std::string_view iconPath) const;
        void SetPosition(int x, int y) const;
        void SetSize(unsigned int w, unsigned int h) const;
        void Show(bool show) const;

        std::string name;
        HWND handle{ nullptr };
        HDC deviceContext{ nullptr };
        int posX;
        int posY;
        unsigned int width;
        unsigned int height;

        KeyDelegate keyDelegate;
        CharDelegate charDelegate;
        MouseButtonDelegate mouseButtonDelegate;
        CursorPosDelegate cursorPosDelegate;
        ScrollDelegate scrollDelegate;
        WindowPosDelegate windowPosDelegate;
        WindowSizeDelegate windowSizeDelegate;
        SuspendDelegate suspendDelegate;
        ResumeDelegate resumeDelegate;
    };
}
