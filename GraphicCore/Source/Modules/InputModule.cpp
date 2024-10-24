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
