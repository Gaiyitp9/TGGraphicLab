/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Editor/MainWindow.h"
#include "Input/Event.h"

namespace TG
{
    MainWindow::MainWindow(int x, int y, int width, int height, std::string_view name)
        : Window(x, y, width, height, name, WindowType::Default)
    {
        SetKeyCallback([&](Input::KeyCode key, int scanCode, Input::Action action) {
            Input::KeyboardEvent keyboardEvent;
            keyboardEvent.key = key;
            if (action == Input::Action::Press || action == Input::Action::Repeat)
                keyboardEvent.isPressed = true;
            m_eventDispatcher.Dispatch(keyboardEvent);
        });
        SetCharCallback([&](char16_t c) {
            Input::CharacterEvent characterEvent;
            characterEvent.c = c;
            m_eventDispatcher.Dispatch(characterEvent);
        });
        SetMouseButtonCallback([&](Input::KeyCode mouseButton, Input::Action action) {
            Input::MouseEvent mouseEvent;
            mouseEvent.button = mouseButton;
            if (action == Input::Action::Press)
                mouseEvent.isPressed = true;
            m_eventDispatcher.Dispatch(mouseEvent);
        });
        SetScrollCallback([&](int xOffset, int yOffset) {
            Input::MouseEvent mouseEvent;
            mouseEvent.wheelDelta = static_cast<short>(yOffset);
            m_eventDispatcher.Dispatch(mouseEvent);
        });
        SetCursorPosCallback([&](int posX, int posY) {
            Input::MouseEvent mouseEvent;
            mouseEvent.x = static_cast<short>(posX);
            mouseEvent.y = static_cast<short>(posY);
            m_eventDispatcher.Dispatch(mouseEvent);
        });
        SetWindowPosCallback([](int xPos, int yPos){});
        SetWindowSizeCallback([](unsigned int w, unsigned int h){});
        SetSuspendCallback([]{});
        SetResumeCallback([]{});
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
