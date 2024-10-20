/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Module.h"
#include "Base/MainWindow.h"
#include "Base/Chronometer.h"
#include "Base/EventHandler.hpp"
#include "Base/TypeList.hpp"
#include "Input/Event.h"

namespace TG
{
    // Platform模块输出的事件列表
    // 鼠标事件
    // 键盘事件
    // 窗口状态事件
    using PlatformEventList = TypeList<Input::Event<Input::Mouse>, Input::Event<Input::Keyboard>>;

    class PlatformModule final : public Module
    {
        using EventDispatcher = EventDispatcherFromEventList<PlatformEventList>;

    public:
        PlatformModule();
        PlatformModule(const PlatformModule&) = delete;
        PlatformModule(PlatformModule&&) = delete;
        PlatformModule& operator=(const PlatformModule&) = delete;
        PlatformModule& operator=(PlatformModule&&) = delete;
        ~PlatformModule() override;

        void Update() override;

        [[nodiscard]] bool ShouldExit() const { return m_mainWindow.IsDestroyed(); }
        [[nodiscard]] int ExitCode() const { return *m_exitCode; }
        [[nodiscard]] const Window& GetWindow() const { return m_mainWindow; }

        template<typename Event> requires PlatformEventList::Contains<Event>
        void AddEventListener(IEventHandler<Event>& handler) { m_eventDispatcher.Register(handler); }
        template<typename Event> requires PlatformEventList::Contains<Event>
        void RemoveEventListener(const IEventHandler<Event>& handler) { m_eventDispatcher.UnRegister(handler); }

    private:
        // 主显示器的尺寸
        int m_screenWidth{ 0 };
        int m_screenHeight{ 0 };
        // 主窗口
        MainWindow m_mainWindow;
        std::optional<int> m_exitCode;
        // 事件分发器
        EventDispatcher m_eventDispatcher;
        // 高精度计时器
        Chronometer m_timer;
    };
}
