/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "InputEvent.hpp"
#include "../Math/Vector.hpp"
#include <queue>
#include <bitset>

namespace LCH
{
	class Mouse
	{
	public:
		Mouse();
		~Mouse();

		void Update();

		void OnMouseMove(int x, int y);
		void OnButtonPress(KeyCode key);
		void OnButtonRelease(KeyCode key);
		void OnWheelRoll(KeyCode key, short delta);

		const Math::Vector2i& Position() const noexcept;
		short RawWheelDelta() const noexcept;			// 鼠标滚轮变化值是WHEEL_DELTA的整数倍
		short WheelDelta() const noexcept;				// 两个函数分别取没有除以和除以WHEEL_DELTA后的变化值

		static bool IsMouseCode(KeyCode code);

	private:
		void TrimEventBuffer();
		void SpyMouseEvent(InputEvent e);

	public:
		std::bitset<8> mouseStates;						// 按键状态(是否被按下)
		std::bitset<8> mouseDown;						// 按键是否刚刚按下
		std::bitset<8> mouseUp;							// 按键是否刚刚松开

		bool spyMouse = false;							// 是否监控鼠标

	private:
		Math::Vector2i position;								// 鼠标位置
		short wheelDelta = 0;							// 滚轮变化值，正值表示向前滚动，远离使用者；负值表示向后滚动，朝向使用者

		std::queue<InputEvent> eventBuffer;				// 输入事件队列
		static constexpr unsigned int BUFSIZE = 16u;	// 队列最大长度
	};
}