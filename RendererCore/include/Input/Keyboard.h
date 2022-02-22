/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

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

	private:
		static constexpr unsigned int nKeys = 256u;
		std::bitset<nKeys> keyStates;		// 按键状态
		std::queue<InputEvent> events;		// 输入事件队列
		std::queue<char> chars;				// 输入字符队列
	};
}