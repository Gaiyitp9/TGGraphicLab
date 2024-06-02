/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Editor/MainWindow.h"

namespace TG
{
    MainWindow::MainWindow(int x, int y, int width, int height, std::string_view name, Window const *parent)
        : Window(x, y, width, height, name, PAL::WindowType::Default, parent)
    {
        m_window.SetKeyCallback([](PAL::Key key, int scanCode, PAL::InputAction action){});
        m_window.SetCharCallback([](char16_t c){});
        m_window.SetMouseButtonCallback([](PAL::MouseButton mouseButton, PAL::InputAction action){});
        m_window.SetScrollCallback([](int xOffset, int yOffset){});
        m_window.SetCursorPosCallback([](int xPos, int yPos){});
        m_window.SetWindowPosCallback([](int xPos, int yPos){});
        m_window.SetWindowSizeCallback([](unsigned int w, unsigned int h){});
        m_window.SetSuspendCallback([]{});
        m_window.SetResumeCallback([]{});
    }

    MainWindow::~MainWindow() = default;
}
