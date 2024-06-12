/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Window.h"
#include "Input/EventDispatcher.h"

namespace TG
{
    class MainWindow final : public Window
    {
    public:
        MainWindow(int x, int y, int width, int height, std::string_view name, Window const* parent = nullptr);
        MainWindow(const MainWindow&) = delete;
        MainWindow(MainWindow&&) = delete;
        MainWindow& operator=(const MainWindow&) = delete;
        MainWindow& operator=(MainWindow&&) = delete;
        ~MainWindow() override = default;

        // 添加输入事件监听器(比如记录输入状态的管理器)
        void AddInputEventListener(Input::IEventHandler& handler);
        void RemoveInputEventListener(const Input::IEventHandler& handler);

    private:
        Input::EventDispatcher m_eventDispatcher;
    };
}
