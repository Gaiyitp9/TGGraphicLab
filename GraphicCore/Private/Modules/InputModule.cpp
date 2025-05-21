/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Modules/InputModule.h"
#include "Input/Devices.h"

namespace TG
{
    InputModule::InputModule()
    {
        m_mouse = std::make_shared<Input::Mouse>();
        m_keyboard = std::make_shared<Input::Keyboard>();
        Input::Devices::Self().m_mouse = m_mouse;
        Input::Devices::Self().m_keyboard = m_keyboard;
    }

    InputModule::~InputModule() = default;

    void InputModule::Subscribe(MulticastDelegate<void(const Input::Event<Input::Mouse>&)>& mouseEventDelegate)
    {
        std::weak_ptr mouse(m_mouse);
        mouseEventDelegate.Add([mouse](const Input::Event<Input::Mouse>& event) {
            if (auto mousePtr = mouse.lock())
                mousePtr->Handle(event);
        });
    }

    void InputModule::Subscribe(MulticastDelegate<void(const Input::Event<Input::Keyboard>&)>& keyboardEventDelegate)
    {
        std::weak_ptr keyboard(m_keyboard);
        keyboardEventDelegate.Add([keyboard](const Input::Event<Input::Keyboard>& event) {
            if (auto keyboardPtr = keyboard.lock())
                keyboardPtr->Handle(event);
        });
    }

    void InputModule::Update()
    {

    }

    void InputModule::PostUpdate()
    {
        m_mouse->Update();
        m_keyboard->Update();
    }
}
