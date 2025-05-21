/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Input/Devices.h"

namespace TG::Input
{
    bool Devices::GetKey(KeyCode key) const
    {
        return (!m_mouse.expired() && m_mouse.lock()->GetKey(key)) ||
            (!m_keyboard.expired() && m_keyboard.lock()->GetKey(key));
    }

    bool Devices::GetKeyDown(KeyCode key) const
    {
        return (!m_mouse.expired() && m_mouse.lock()->GetKeyDown(key)) ||
            (!m_keyboard.expired() && m_keyboard.lock()->GetKeyDown(key));
    }

    bool Devices::GetKeyUp(KeyCode key) const
    {
        return (!m_mouse.expired() && m_mouse.lock()->GetKeyUp(key)) ||
            (!m_keyboard.expired() && m_keyboard.lock()->GetKeyUp(key));
    }

    short Devices::MousePositionX() const
    {
        if (auto mouse = m_mouse.lock())
            return mouse->PositionX();
        return 0;
    }

    short Devices::MousePositionY() const
    {
        if (auto mouse = m_mouse.lock())
            return mouse->PositionY();
        return 0;
    }

    short Devices::MouseWheelDelta() const
    {
        if (auto mouse = m_mouse.lock())
            return mouse->WheelDelta();
        return 0;
    }
}
