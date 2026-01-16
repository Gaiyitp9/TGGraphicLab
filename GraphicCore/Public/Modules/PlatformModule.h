/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Module.h"
#include "Base/MainWindow.h"
#include "Base/Chronometer.h"
#include "Input/Mouse.h"
#include "Input/Keyboard.h"

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
        [[nodiscard]] const IDefaultVideoPort& GetVideoPort() const { return m_mainWindow; }
        [[nodiscard]] const ITimer& GetTimer() const { return m_timer; };

        // 窗口尺寸变化委托
        MulticastDelegate<void(unsigned, unsigned)> onWindowResize;

    private:
        // 主显示器的尺寸
        int m_screenWidth{ 0 };
        int m_screenHeight{ 0 };
        // 主窗口
        MainWindow m_mainWindow;
        std::optional<int> m_exitCode;
        // 高精度计时器
        Chronometer m_timer;
        // 输入设备
        Input::Mouse m_mouse;
        Input::Keyboard m_keyboard;
    };
}
