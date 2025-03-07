/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Module.h"
#include "Input/Mouse.h"
#include "Input/Keyboard.h"
#include "Base/Delegate.hpp"

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

        void Subscribe(MulticastDelegate<void(const Input::Event<Input::Mouse>&)>& mouseEventDelegate);
        void Subscribe(MulticastDelegate<void(const Input::Event<Input::Keyboard>&)>& keyboardEventDelegate);

        void Update() override;
        void PostUpdate() override;

        [[nodiscard]] bool GetKey(Input::KeyCode key) const;
        [[nodiscard]] bool GetKeyDown(Input::KeyCode key) const;
        [[nodiscard]] bool GetKeyUp(Input::KeyCode key) const;

    private:
        Input::Mouse m_mouse;
        Input::Keyboard m_keyboard;
    };
}
