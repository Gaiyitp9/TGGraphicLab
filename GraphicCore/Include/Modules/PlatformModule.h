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

        void PreUpdate() override;
        void Update() override;
        void PostUpdate() override;

        [[nodiscard]] bool ShouldExit() const;
        [[nodiscard]] int ExitCode() const;
        [[nodiscard]] const Window& GetWindow() const { return m_mainWindow; }

        std::function<void(const Input::Event<Input::Mouse>&)>& OnMouseEvent() { return m_mouseEventDelegate; }
        std::function<void(const Input::Event<Input::Keyboard>&)>& OnKeyboardEvent() { return m_keyboardEventDelegate; }

    private:
        // 主显示器的尺寸
        int m_screenWidth{ 0 };
        int m_screenHeight{ 0 };
        // 主窗口
        MainWindow m_mainWindow;
        std::optional<int> m_exitCode;
        // 高精度计时器
        Chronometer m_timer;
        // 鼠标事件委托
        std::function<void(const Input::Event<Input::Mouse>&)> m_mouseEventDelegate;
        // 键盘事件委托
        std::function<void(const Input::Event<Input::Keyboard>&)> m_keyboardEventDelegate;
    };
}
