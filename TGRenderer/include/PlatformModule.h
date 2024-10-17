/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Editor/MainWindow.h"
#include "Base/Chronometer.h"
#include "Base/EventHandler.hpp"
#include "Base/TypeList.hpp"
#include "Input/Event.h"
#include <memory>
#include <string_view>

namespace TG
{
    // Platform模块输出的事件列表
    // · 鼠标事件
    // · 键盘事件
    // · 窗口状态事件
    using PlatformEventList = TypeList<Input::Event<Input::Mouse>, Input::Event<Input::Keyboard>>;

    // template<typename Event>
    // constexpr bool IsPlatformSupportedEvent = std::is_same_v<E

    class PlatformModule
    {
        using EventDispatcher = EventDispatcherFromEventList<PlatformEventList>;

    public:
        PlatformModule();
        PlatformModule(const PlatformModule&) = delete;
        PlatformModule(PlatformModule&&) = delete;
        PlatformModule& operator=(const PlatformModule&) = delete;
        PlatformModule& operator=(PlatformModule&&) = delete;
        ~PlatformModule();

        void Update();

        template<typename Event>
        void AddEventListener(IEventHandler<Event>& handler) { m_eventDispatcher.Register(handler); }
        template<typename Event>
        void RemoveEventListener(const IEventHandler<Event>& handler) { m_eventDispatcher.UnRegister(handler); }

    private:
        // 主显示器的尺寸
        int m_screenWidth{ 0 };
        int m_screenHeight{ 0 };
        // 主窗口的位置
        int m_windowPosX{ 0 };
        int m_windowPosY{ 0 };
        // 主窗口的尺寸
        int m_windowWidth{ 0 };
        int m_windowHeight{ 0 };
        // 窗口标题
        std::string_view m_windowTitle{ "天工渲染器" };
        // 主窗口
        std::unique_ptr<MainWindow> m_mainWindow;
        // 事件分发器
        EventDispatcher m_eventDispatcher;
        // 高精度计时器
        Chronometer m_timer;
    };
}
