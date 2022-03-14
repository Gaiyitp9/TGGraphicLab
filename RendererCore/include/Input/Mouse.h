/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "../SlimWindows.h"
#include "InputEvent.h"
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

		void OnMouseMove(const POINTS& position);
		void OnButtonPress(KeyCode key);
		void OnButtonRelease(KeyCode key);

		const POINTS& Position() const noexcept;

	public:
		std::bitset<8> mouseStates;						// 按键状态(是否被按下)
		std::bitset<8> mouseDown;						// 按键是否刚刚按下
		std::bitset<8> mouseUp;							// 按键是否刚刚松开

		bool spyMouse = false;							// 是否监控鼠标

	private:
		POINTS position{0};								// 鼠标位置

		std::queue<InputEvent> eventBuffer;				// 输入事件队列
		static constexpr unsigned int BUFSIZE = 16u;	// 队列最大长度
	};
}