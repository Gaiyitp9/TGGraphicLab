/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Input/Keyboard.h"

namespace TG::Input
{
	void Keyboard::Update()
	{
		m_keyDown.reset();
		m_keyUp.reset();
	}

    void Keyboard::Handle(const Event<Keyboard> &event)
    {
	    // 上面已经判断过类型了，所以这里不用dynamic_cast，提升性能
	    const auto key = static_cast<std::size_t>(event.key);
	    if (event.isPressed)
	    {
	    	// 第一次按下，key down为true
	        if (!m_keyHold.test(key))
				m_keyDown[key] = true;
	        m_keyHold[key] = true;
	    }
	    else
	    {
	    	// 放开按键，key up为true(不存在第一次放开，每一次WM_KEYUP都是第一次放开)
	        m_keyUp[key] = true;
	        m_keyHold[key] = false;
	    }
    }

    bool Keyboard::GetKey(KeyCode key) const
    {
		return key >= KeyCode::Backspace && key <= KeyCode::Quote && m_keyHold.test(static_cast<std::size_t>(key));
    }

    bool Keyboard::GetKeyDown(KeyCode key) const
    {
		return key >= KeyCode::Backspace && key <= KeyCode::Quote && m_keyDown.test(static_cast<std::size_t>(key));
    }

    bool Keyboard::GetKeyUp(KeyCode key) const
    {
		return key >= KeyCode::Backspace && key <= KeyCode::Quote && m_keyUp.test(static_cast<std::size_t>(key));
    }
}
