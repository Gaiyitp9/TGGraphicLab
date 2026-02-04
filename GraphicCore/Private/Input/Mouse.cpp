/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Input/Mouse.h"

namespace TG::Input
{
	void Mouse::Update()
	{
		m_mouseDown.reset();
		m_mouseUp.reset();
		m_wheelDelta = 0;
	}

    void Mouse::Handle(const Event<Mouse>& event)
    {
		if (event.key != KeyCode::None)
		{
	        const auto key = static_cast<std::size_t>(event.key);
			if (event.isPressed)
			{
				if (!m_mouseHold.test(key))
					m_mouseDown[key] = true;
				m_mouseHold[key] = true;
			}
			else
			{
				m_mouseUp[key] = true;
				m_mouseHold[key] = false;
			}
		}

		m_positionX = event.x;
		m_positionY = event.y;
		m_wheelDelta += event.wheelDelta;
    }

    bool Mouse::GetKey(KeyCode key) const
    {
		return key <= KeyCode::MiddleMouseButton && m_mouseHold.test(static_cast<std::size_t>(key));
    }

    bool Mouse::GetKeyDown(KeyCode key) const
    {
        return key <= KeyCode::MiddleMouseButton && m_mouseDown.test(static_cast<std::size_t>(key));
    }

    bool Mouse::GetKeyUp(KeyCode key) const
    {
        return key <= KeyCode::MiddleMouseButton && m_mouseUp.test(static_cast<std::size_t>(key));
    }
}
