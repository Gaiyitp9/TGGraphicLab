/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Modules/PlatformModule.h"
#include "Diagnostic/Log.h"

namespace TG
{
    PlatformModule::PlatformModule() : m_mainWindow(0, 0, 1, 1, "天工渲染器")
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
        Log::Instance().Info(Chronometer::Date());

        int windowWidth = m_screenWidth * 3 / 4;
        int windowHeight = m_screenHeight * 4 / 5;
        int windowPosX = windowWidth / 8;
        int windowPosY = windowHeight / 10;
        m_mainWindow.SetPosition(windowPosX, windowPosY);
        m_mainWindow.SetSize(windowWidth, windowHeight);
        m_mainWindow.Show(true);
        m_mainWindow.SetIcon("maple-leaf.ico");
        m_mainWindow.SetKeyCallback(
            [&keyboardEventDelegate = m_keyboardEventDelegate](Input::KeyCode key, int scanCode, Input::Action action) {
                Input::Event<Input::Keyboard> event;
                event.key = key;
                if (action == Input::Action::Press || action == Input::Action::Repeat)
                    event.isPressed = true;
                keyboardEventDelegate(event);
            }
        );
        m_mainWindow.SetCharCallback([&keyboardEventDelegate = m_keyboardEventDelegate](char16_t c) {
            Input::Event<Input::Keyboard> event;
            event.c = c;
            keyboardEventDelegate(event);
        });
        m_mainWindow.SetMouseButtonCallback(
            [&mouseEventDelegate = m_mouseEventDelegate](Input::KeyCode mouseButton, Input::Action action) {
                Input::Event<Input::Mouse> event;
                event.key = mouseButton;
                if (action == Input::Action::Press)
                    event.isPressed = true;
                mouseEventDelegate(event);
            }
        );
        m_mainWindow.SetScrollCallback([&mouseEventDelegate = m_mouseEventDelegate](int xOffset, int yOffset) {
            Input::Event<Input::Mouse> event;
            event.wheelDelta = static_cast<short>(yOffset);
            mouseEventDelegate(event);
        });
        m_mainWindow.SetCursorPosCallback([&mouseEventDelegate = m_mouseEventDelegate](int posX, int posY) {
            Input::Event<Input::Mouse> event;
            event.x = static_cast<short>(posX);
            event.y = static_cast<short>(posY);
            mouseEventDelegate(event);
        });
        m_mainWindow.SetWindowPosCallback([](int xPos, int yPos){});
        m_mainWindow.SetWindowSizeCallback([](unsigned int w, unsigned int h){});
        m_mainWindow.SetSuspendCallback([&timer = m_timer]{ timer.Pause(); });
        m_mainWindow.SetResumeCallback([&timer = m_timer]{ timer.Start(); });
    }

    PlatformModule::~PlatformModule() = default;

    void PlatformModule::PreUpdate()
    {
        m_exitCode = Window::PollEvents();
    }

    void PlatformModule::Update()
    {

    }

    void PlatformModule::PostUpdate()
    {

    }

    bool PlatformModule::ShouldExit() const
    {
        return m_mainWindow.IsDestroyed();
    }

    int PlatformModule::ExitCode() const
    {
        return *m_exitCode;
    }

}
