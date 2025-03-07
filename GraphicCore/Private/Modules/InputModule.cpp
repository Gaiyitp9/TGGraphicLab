/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Modules/InputModule.h"

namespace TG
{
    InputModule::InputModule() = default;

    InputModule::~InputModule() = default;

    void InputModule::Subscribe(MulticastDelegate<void(const Input::Event<Input::Mouse>&)>& mouseEventDelegate)
    {
        mouseEventDelegate.Add([&mouse = m_mouse](const Input::Event<Input::Mouse>& event) {
            mouse.Handle(event);
        });
    }

    void InputModule::Subscribe(MulticastDelegate<void(const Input::Event<Input::Keyboard>&)>& keyboardEventDelegate)
    {
        keyboardEventDelegate.Add([&keyboard = m_keyboard](const Input::Event<Input::Keyboard>& event) {
            keyboard.Handle(event);
        });
    }

    void InputModule::Update()
    {

    }

    void InputModule::PostUpdate()
    {
        m_mouse.Update();
        m_keyboard.Update();
    }

    bool InputModule::GetKey(Input::KeyCode key) const
    {
        return m_mouse.GetKey(key) || m_keyboard.GetKey(key);
    }

    bool InputModule::GetKeyDown(Input::KeyCode key) const
    {
        return m_mouse.GetKeyDown(key) || m_keyboard.GetKeyDown(key);
    }

    bool InputModule::GetKeyUp(Input::KeyCode key) const
    {
        return m_mouse.GetKeyUp(key) || m_keyboard.GetKeyUp(key);
    }
}
