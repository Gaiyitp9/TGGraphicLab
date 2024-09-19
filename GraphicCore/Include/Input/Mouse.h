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
	class Mouse
	{
	public:
		Mouse() = default;
		Mouse(const Mouse&) = delete;
		Mouse& operator=(const Mouse&) = delete;
		Mouse(Mouse&&) = delete;
		Mouse& operator=(Mouse&&) = delete;
		~Mouse() = default;

		void Update();
        void Receive(const Event& e);
        [[nodiscard]] bool GetKey(KeyCode k) const;
        [[nodiscard]] bool GetKeyDown(KeyCode k) const;
        [[nodiscard]] bool GetKeyUp(KeyCode k) const;

        [[nodiscard]] short PositionX() const noexcept { return m_positionX; }
        [[nodiscard]] short PositionY() const noexcept { return m_positionY; }
        [[nodiscard]] short WheelDelta() const noexcept { return m_wheelDelta; }

	private:
		static bool IsMouseKey(KeyCode k)
		{
			if (const auto key = static_cast<std::size_t>(k); key <= 0x04)
				return true;
			return false;
		}

		std::bitset<8> m_mouseDown;     // 按键是否刚刚按下
		std::bitset<8> m_mouseUp;       // 按键是否刚刚松开
		std::bitset<8> m_mouseHold;     // 按键是否按住

        short m_positionX{ 0 };			// 鼠标位置
        short m_positionY{ 0 };
		short m_wheelDelta{ 0 };		// 滚轮变化值，正值表示向前滚动，远离使用者；负值表示向后滚动，朝向使用者
	};
}
