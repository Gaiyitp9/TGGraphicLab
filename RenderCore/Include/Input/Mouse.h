/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "../Math/Core.hpp"
#include "Event.h"
#include <bitset>

namespace TG::Input
{
    // 鼠标
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
        void SpyEvent(bool enable);
        [[nodiscard]] bool GetKey(KeyCode k) const;
        [[nodiscard]] bool GetKeyDown(KeyCode k) const;
        [[nodiscard]] bool GetKeyUp(KeyCode k) const;

        [[nodiscard]] const Math::Vector2i& Position() const noexcept { return m_position; }
        // 鼠标滚轮变化值是WHEEL_DELTA的整数倍
        [[nodiscard]] short RawWheelDelta() const noexcept { return m_wheelDelta; }
        // 两个函数分别取没有除以和除以WHEEL_DELTA后的变化值
        [[nodiscard]] short WheelDelta() const noexcept { return static_cast<short>(m_wheelDelta); }

	private:
        static bool IsMouseKey(KeyCode k)
        {
            auto key = static_cast<std::size_t>(k);
            if (key <= 0x04)
                return true;
            return false;
        }

		void SpyMouseEvent(const Event& e);

		std::bitset<8> m_mouseDown;     // 按键是否刚刚按下
		std::bitset<8> m_mouseUp;       // 按键是否刚刚松开

        Math::Vector2i m_position;      // 鼠标位置
		short m_wheelDelta = 0;         // 滚轮变化值，正值表示向前滚动，远离使用者；负值表示向后滚动，朝向使用者
        bool m_spyEvent = false;
	};
}
