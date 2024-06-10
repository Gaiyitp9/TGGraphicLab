/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Input/Mouse.h"
#include <typeinfo>

namespace TG::Input
{
	void Mouse::Update()
	{
		m_mouseDown.reset();
		m_mouseUp.reset();
	}

    void Mouse::Receive(const Event& e)
    {
        // 不处理非鼠标事件
        if (typeid(e) != typeid(MouseEvent)) return;

		// 上面已经判断过类型了，所以这里不用dynamic_cast，提升性能
        const auto& mouseEvent = static_cast<const MouseEvent&>(e);
		if (mouseEvent.button != KeyCode::None)
		{
	        const auto key = static_cast<std::size_t>(mouseEvent.button);
			if (mouseEvent.isPressed)
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

		m_positionX = mouseEvent.x;
		m_positionY = mouseEvent.y;
		m_wheelDelta = mouseEvent.wheelDelta;
    }

    bool Mouse::GetKey(KeyCode k) const
    {
        if (!IsMouseKey(k)) return false;

        const auto pos = static_cast<std::size_t>(k);
        return m_mouseHold.test(pos);
    }

    bool Mouse::GetKeyDown(KeyCode k) const
    {
        if (!IsMouseKey(k)) return false;

        const auto pos = static_cast<std::size_t>(k);
        return m_mouseDown.test(pos);
    }

    bool Mouse::GetKeyUp(KeyCode k) const
    {
        if (!IsMouseKey(k)) return false;

        const auto pos = static_cast<std::size_t>(k);
        return m_mouseUp.test(pos);
    }
}
