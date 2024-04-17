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
		Keyboard();
		~Keyboard();

		void Update();
        void Receive(const Event& e);
        void SpyEvent(bool enable);
		[[nodiscard]] bool GetKey(KeyCode k) const;
		[[nodiscard]] bool GetKeyDown(KeyCode k) const;
		[[nodiscard]] bool GetKeyUp(KeyCode k) const;

	private:
        static bool IsKeyBoardKey(KeyCode k)
        {
            auto key = static_cast<std::size_t>(k);
            if (key >= 0x08 && key <= 0xDE)
                return true;
            return false;
        }

		std::bitset<256> m_keyHold;					    // 按键状态(是否被按下)
		std::bitset<256> m_keyDown;					    // 按键是否刚刚按下
		std::bitset<256> m_keyUp;						// 按键是否刚刚松开

		bool m_spyEvent = false;						// 是否监控键盘
	};
}