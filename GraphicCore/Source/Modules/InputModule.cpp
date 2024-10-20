/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Modules/InputModule.h"

namespace TG
{
    InputModule::InputModule() = default;

    InputModule::~InputModule() = default;

    void InputModule::Update()
    {
        m_mouse.Update();
        m_keyboard.Update();
    }

    bool InputModule::GetKey(Input::KeyCode key) const
    {
        if (key <= Input::KeyCode::MiddleMouseButton)
            return m_mouse.GetKey(key);

        if (key >= Input::KeyCode::Backspace && key <= Input::KeyCode::Quote)
            return m_keyboard.GetKey(key);

        return false;
    }

    bool InputModule::GetKeyDown(Input::KeyCode key) const
    {
        if (key <= Input::KeyCode::MiddleMouseButton)
            return m_mouse.GetKeyDown(key);

        if (key >= Input::KeyCode::Backspace && key <= Input::KeyCode::Quote)
            return m_keyboard.GetKeyDown(key);

        return false;
    }

    bool InputModule::GetKeyUp(Input::KeyCode key) const
    {
        if (key <= Input::KeyCode::MiddleMouseButton)
            return m_mouse.GetKeyUp(key);

        if (key >= Input::KeyCode::Backspace && key <= Input::KeyCode::Quote)
            return m_keyboard.GetKeyUp(key);

        return false;
    }
}
