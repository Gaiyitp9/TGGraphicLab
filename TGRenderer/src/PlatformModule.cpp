/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "PlatformModule.h"
#include "spdlog/spdlog.h"

namespace TG
{
    PlatformModule::PlatformModule()
    {
        // 注：使用CRT library检测内存泄漏时，文件的行分隔符要设置为CRLF(\r\n)，否则_CrtSetDbgFlag函数不起作用
        // 开启内存泄漏检测
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        // 设置内存泄漏消息输出到控制台
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
        // 为了能在控制台查看日志，需要把控制台的代码页(code page)设置为UTF-8
        SetConsoleCP(65001);
        SetConsoleOutputCP(65001);
        // 获取当前显示器的宽和高
        m_screenWidth = GetSystemMetrics(SM_CXSCREEN);
        m_screenHeight = GetSystemMetrics(SM_CYSCREEN);

        // 关闭错误流
        // FILE* nullFile = nullptr;
        // freopen_s(&nullFile, "NUL", "w", stderr);

        // 打印日期
        spdlog::info(Chronometer::Date());

        m_windowWidth = m_screenWidth * 3 / 4;
        m_windowHeight = m_screenHeight * 4 / 5;
        m_windowPosX = m_windowWidth / 8;
        m_windowPosY = m_windowHeight / 10;
        m_mainWindow = std::make_unique<MainWindow>(m_windowPosX, m_windowPosY,
            m_windowWidth, m_windowHeight, m_windowTitle);
        m_mainWindow->SetIcon("maple-leaf.ico");

        m_mainWindow->SetKeyCallback([&](Input::KeyCode key, int scanCode, Input::Action action) {
            Input::Event<Input::Keyboard> event;
            event.key = key;
            if (action == Input::Action::Press || action == Input::Action::Repeat)
                event.isPressed = true;
            m_eventDispatcher.Dispatch(event);
        });
        m_mainWindow->SetCharCallback([&](char16_t c) {
            Input::Event<Input::Keyboard> event;
            event.c = c;
            m_eventDispatcher.Dispatch(event);
        });
        m_mainWindow->SetMouseButtonCallback([&](Input::KeyCode mouseButton, Input::Action action) {
            Input::Event<Input::Mouse> event;
            event.key = mouseButton;
            if (action == Input::Action::Press)
                event.isPressed = true;
            m_eventDispatcher.Dispatch(event);
        });
        m_mainWindow->SetScrollCallback([&](int xOffset, int yOffset) {
            Input::Event<Input::Mouse> event;
            event.wheelDelta = static_cast<short>(yOffset);
            m_eventDispatcher.Dispatch(event);
        });
        m_mainWindow->SetCursorPosCallback([&](int posX, int posY) {
            Input::Event<Input::Mouse> event;
            event.x = static_cast<short>(posX);
            event.y = static_cast<short>(posY);
            m_eventDispatcher.Dispatch(event);
        });
        m_mainWindow->SetWindowPosCallback([](int xPos, int yPos){});
        m_mainWindow->SetWindowSizeCallback([](unsigned int w, unsigned int h){});
        m_mainWindow->SetSuspendCallback([]{});
        m_mainWindow->SetResumeCallback([]{});
    }

    PlatformModule::~PlatformModule() = default;

    void PlatformModule::Update()
    {
        while (true)
		{
			if (m_mainWindow->IsDestroyed() || Window::PollEvents())
				break;
		}
    }
}
