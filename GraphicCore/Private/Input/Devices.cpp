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
        return (m_mouse && m_mouse->GetKey(key)) ||
            (m_keyboard && m_keyboard->GetKey(key));
    }

    bool Devices::GetKeyDown(KeyCode key) const
    {
        return (m_mouse && m_mouse->GetKeyDown(key)) ||
            (m_keyboard && m_keyboard->GetKeyDown(key));
    }

    bool Devices::GetKeyUp(KeyCode key) const
    {
        return (m_mouse && m_mouse->GetKeyUp(key)) ||
            (m_keyboard && m_keyboard->GetKeyUp(key));
    }

    short Devices::MousePositionX() const
    {
        if (m_mouse)
            return m_mouse->PositionX();
        return 0;
    }

    short Devices::MousePositionY() const
    {
        if (m_mouse)
            return m_mouse->PositionY();
        return 0;
    }

    short Devices::MouseWheelDelta() const
    {
        if (m_mouse)
            return m_mouse->WheelDelta();
        return 0;
    }
}
