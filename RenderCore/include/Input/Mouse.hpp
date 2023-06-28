/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "../Math/Core.hpp"
#include "Event.hpp"
#include <bitset>

namespace TG::Input
{
    // 鼠标
	class Mouse : public Device
	{
	public:
		Mouse();
		~Mouse() override;

		void Update() override;
        void Receive(const Event& e) override;
        void SpyEvent(bool enable) override;
        bool GetKey(KeyCode k) override;
        bool GetKeyDown(KeyCode k) override;
        bool GetKeyUp(KeyCode k) override;

        [[nodiscard]] const Math::Vector2i& Position() const noexcept { return m_position; }
        // 鼠标滚轮变化值是WHEEL_DELTA的整数倍
        [[nodiscard]] short RawWheelDelta() const noexcept { return m_wheelDelta; }
        // 两个函数分别取没有除以和除以WHEEL_DELTA后的变化值
        [[nodiscard]] short WheelDelta() const noexcept { return static_cast<short>(m_wheelDelta / WHEEL_DELTA); }

	private:
		void SpyMouseEvent(Event e);

	public:
		std::bitset<8> mouseHold;       // 按键状态(是否被按下)
		std::bitset<8> mouseDown;       // 按键是否刚刚按下
		std::bitset<8> mouseUp;         // 按键是否刚刚松开

	private:
		Math::Vector2i m_position;      // 鼠标位置
		short m_wheelDelta = 0;         // 滚轮变化值，正值表示向前滚动，远离使用者；负值表示向后滚动，朝向使用者
        bool m_spyMouse = false;
	};
}