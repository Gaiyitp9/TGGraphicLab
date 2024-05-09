/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#ifdef _WIN64
#include "Windows/WindowCore.h"
#endif

namespace TG::PAL
{
    // 代表不同平台中的原生窗口
    class NativeWindow;
    // 窗口基类
    class Window : public NativeWindow
    {
        using KeyFunction = std::function<void(int key, int scancode, int action, int mods)>;
        using MouseButtonFunction =  std::function<void(int button, int action, int mods)>;

    public:
        Window(int x, int y, int width, int height, wchar_t const* title);

        KeyFunction SetKeyCallback(KeyFunction function);
        MouseButtonFunction SetMouseButtonCallback(MouseButtonFunction function);
    private:
        std::string m_windowName;
        KeyFunction m_keyFunction;
        MouseButtonFunction m_mouseButtonFunction;
    };
    // 轮询输入事件，需要每帧都调用
    void PollEvents();
}
