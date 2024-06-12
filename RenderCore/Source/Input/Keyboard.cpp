/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Input/Keyboard.h"
#include <typeinfo>

namespace TG::Input
{
	void Keyboard::Update()
	{
		m_keyDown.reset();
		m_keyUp.reset();
	}

    void Keyboard::Receive(const Event &e)
    {
	    // 不处理非键盘事件
        if (typeid(e) != typeid(KeyboardEvent)) return;

	    // 上面已经判断过类型了，所以这里不用dynamic_cast，提升性能
	    const auto& keyboardEvent = static_cast<const KeyboardEvent&>(e);
	    const auto key = static_cast<std::size_t>(keyboardEvent.key);
	    if (keyboardEvent.isPressed)
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

    bool Keyboard::GetKey(KeyCode k) const
    {
        if (!IsKeyBoardKey(k)) return false;

        return m_keyHold.test(static_cast<std::size_t>(k));
    }

    bool Keyboard::GetKeyDown(KeyCode k) const
    {
        if (!IsKeyBoardKey(k)) return false;

        return m_keyDown.test(static_cast<std::size_t>(k));
    }

    bool Keyboard::GetKeyUp(KeyCode k) const
    {
        if (!IsKeyBoardKey(k)) return false;

        return m_keyUp.test(static_cast<std::size_t>(k));
    }
}
