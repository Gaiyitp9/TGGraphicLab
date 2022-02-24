/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "../SlimWindows.h"
#include "KeyCode.h"
#include <bitset>
#include <queue>
#include "InputEvent.h"

namespace LCH
{
	class Keyboard
	{
	public:
		Keyboard();
		~Keyboard();

		void Update();									// 分析事件队列，设置各种状态

		void OnKeyPressed(unsigned char keyCode);
		void OnKeyReleased(unsigned char keyCode);
		void OnChar(char ch);

		WPARAM MapLeftRightKey(WPARAM, LPARAM);			// 映射左右按键(shift, ctrl, alt)

	private:
		static constexpr unsigned int NUMKEYS = 256u;	// 按键数量
		static constexpr unsigned int BUFSIZE = 16u;	// 队列最大长度

	public:
		std::bitset<NUMKEYS> keyStates;					// 按键状态(是否被按下)
		std::bitset<NUMKEYS> keyDown;					// 按键是否刚刚按下
		std::bitset<NUMKEYS> keyUp;						// 按键是否刚刚松开

		std::queue<InputEvent> eventBuffer;				// 输入事件队列
		std::queue<char> charBuffer;					// 输入字符队列

		bool autoRepeat = true;							// 是否记录重复按键
	};
}