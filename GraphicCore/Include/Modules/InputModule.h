/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Module.h"
#include "Input/Mouse.h"
#include "Input/Keyboard.h"

namespace TG
{
    class InputModule final : public Module
    {
    public:
        InputModule();
        InputModule(const InputModule&) = delete;
        InputModule(InputModule&&) = delete;
        InputModule& operator=(const InputModule&) = delete;
        InputModule& operator=(InputModule&&) = delete;
        ~InputModule() override;

        void Update() override;

        [[nodiscard]] bool GetKey(Input::KeyCode key) const;
        [[nodiscard]] bool GetKeyDown(Input::KeyCode key) const;
        [[nodiscard]] bool GetKeyUp(Input::KeyCode key) const;

        IEventHandler<Input::Event<Input::Mouse>>& GetMouseEventHandler() { return m_mouse; }
        IEventHandler<Input::Event<Input::Keyboard>>& GetKeyboardEventHandler() { return m_keyboard; }

    private:
        Input::Mouse m_mouse;
        Input::Keyboard m_keyboard;
    };
}
