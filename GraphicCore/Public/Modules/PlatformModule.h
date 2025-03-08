/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Module.h"
#include "Base/MainWindow.h"
#include "Base/Chronometer.h"
#include "Input/Event.h"
#include "Base/Delegate.hpp"

namespace TG
{
    class PlatformModule final : public Module
    {
    public:
        PlatformModule();
        PlatformModule(const PlatformModule&) = delete;
        PlatformModule(PlatformModule&&) = delete;
        PlatformModule& operator=(const PlatformModule&) = delete;
        PlatformModule& operator=(PlatformModule&&) = delete;
        ~PlatformModule() override;

        void Update() override;
        void PostUpdate() override;

        [[nodiscard]] bool ShouldExit() const;
        [[nodiscard]] int ExitCode() const;
        [[nodiscard]] const WindowBase& GetWindow() const { return m_mainWindow; }

        // 鼠标事件委托
        MulticastDelegate<void(const Input::Event<Input::Mouse>&)> onMouseEvent;
        // 键盘事件委托
        MulticastDelegate<void(const Input::Event<Input::Keyboard>&)> onKeyboardEvent;
        // 窗口尺寸变化委托
        MulticastDelegate<void(unsigned int, unsigned int)> onWindowResize;

    private:
        // 主显示器的尺寸
        int m_screenWidth{ 0 };
        int m_screenHeight{ 0 };
        // 主窗口
        MainWindow m_mainWindow;
        std::optional<int> m_exitCode;
        // 高精度计时器
        Chronometer m_timer;
    };
}
