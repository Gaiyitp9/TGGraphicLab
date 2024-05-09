/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "KeyCode.h"
#include <functional>
#include <memory>

namespace TG::PAL
{
    // 代表不同平台中的原生窗口
    struct NativeWindow;
    // 窗口消息事件回调
    using KeyFunction = std::function<void(Input::KeyCode key, int scancode, int action, int mods)>;
    using MouseButtonFunction =  std::function<void(Input::KeyCode button, int action, int mods)>;
    KeyFunction SetKeyCallback(NativeWindow const* window, KeyFunction function);
    MouseButtonFunction SetMouseButtonCallback(NativeWindow const* window, MouseButtonFunction function);
    // 创建原生窗口
    std::unique_ptr<NativeWindow> CreateNativeWindow(int x, int y, int width, int height, wchar_t const* title);

    // 轮询输入事件，需要每帧都调用
    void PollEvents();
}
