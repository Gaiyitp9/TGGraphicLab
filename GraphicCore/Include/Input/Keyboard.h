/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Event.h"
#include <bitset>

namespace TG::Input
{
	class Keyboard
	{
	public:
		Keyboard() = default;
		Keyboard(const Keyboard&) = delete;
		Keyboard& operator=(const Keyboard&) = delete;
		Keyboard(Keyboard&&) = delete;
		Keyboard& operator=(Keyboard&&) = delete;
		~Keyboard() = default;

		void Update();
        void Receive(const Event& e);
		[[nodiscard]] bool GetKey(KeyCode k) const;
		[[nodiscard]] bool GetKeyDown(KeyCode k) const;
		[[nodiscard]] bool GetKeyUp(KeyCode k) const;

	private:
        static bool IsKeyBoardKey(KeyCode k)
        {
            if (const auto key = static_cast<std::size_t>(k);
            	key >= static_cast<std::size_t>(KeyCode::Backspace) &&
            	key <= static_cast<std::size_t>(KeyCode::Quote))
                return true;
            return false;
        }

		std::bitset<256> m_keyHold;					    // 按键状态(是否被按下)
		std::bitset<256> m_keyDown;					    // 按键是否刚刚按下
		std::bitset<256> m_keyUp;						// 按键是否刚刚松开
	};
}
