/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Editor/MainWindow.h"
#include "Input/Event.h"

namespace TG
{
    MainWindow::MainWindow(int x, int y, int width, int height, std::string_view name, Window const *parent)
        : Window(x, y, width, height, name, PAL::WindowType::Default, parent)
    {
        m_window.SetKeyCallback([&](PAL::Key key, int scanCode, PAL::InputAction action) {
            Input::KeyboardEvent keyboardEvent;
            keyboardEvent.key = Input::ToKeyCode(key);
            if (action == PAL::InputAction::Press || action == PAL::InputAction::Repeat)
                keyboardEvent.isPressed = true;
            m_eventDispatcher.Dispatch(keyboardEvent);
        });
        m_window.SetCharCallback([&](char16_t c) {
            Input::CharacterEvent characterEvent;
            characterEvent.c = c;
            m_eventDispatcher.Dispatch(characterEvent);
        });
        m_window.SetMouseButtonCallback([&](PAL::MouseButton mouseButton, PAL::InputAction action) {
            Input::MouseEvent mouseEvent;
            mouseEvent.button = Input::ToKeyCode(mouseButton);
            if (action == PAL::InputAction::Press)
                mouseEvent.isPressed = true;
            m_eventDispatcher.Dispatch(mouseEvent);
        });
        m_window.SetScrollCallback([&](int xOffset, int yOffset) {
            Input::MouseEvent mouseEvent;
            mouseEvent.wheelDelta = static_cast<short>(yOffset);
            m_eventDispatcher.Dispatch(mouseEvent);
        });
        m_window.SetCursorPosCallback([&](int posX, int posY) {
            Input::MouseEvent mouseEvent;
            mouseEvent.x = static_cast<short>(posX);
            mouseEvent.y = static_cast<short>(posY);
            m_eventDispatcher.Dispatch(mouseEvent);
        });
        m_window.SetWindowPosCallback([](int xPos, int yPos){});
        m_window.SetWindowSizeCallback([](unsigned int w, unsigned int h){});
        m_window.SetSuspendCallback([]{});
        m_window.SetResumeCallback([]{});
    }

    void MainWindow::AddInputEventListener(Input::IEventHandler& handler)
    {
        m_eventDispatcher.Register(handler);
    }

    void MainWindow::RemoveInputEventListener(const Input::IEventHandler& handler)
    {
        m_eventDispatcher.UnRegister(handler);
    }

}
