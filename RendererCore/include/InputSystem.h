/****************************************************************
* Copyright (c) LiaoChenhan. All rights reserved.				*
*																*
* This code is licensed under the MIT License (MIT).			*
* THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF				*
* ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY			*
* IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR				*
* PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.				*
*																*
* Author: LiaoChenhan											*
*****************************************************************/
#pragma once

#include <bitset>
#include <queue>
#include "KeyCode.h"
#include "InputEvent.h"

namespace LCH
{
	class InputSystem
	{
		friend class Window;
	public:
		InputSystem();
		InputSystem(const InputSystem&) = delete;
		InputSystem& operator=(const InputSystem&) = delete;
		~InputSystem();

		void Update();

		bool GetKey(KeyCode key) const noexcept;
		bool GetKeyDown(KeyCode key) const noexcept;
		bool GetKeyUp(KeyCode key) const noexcept;

	private:
		void OnKeyPressed(size_t key);
		void OnKeyReleased(size_t key);

	private:
		static constexpr unsigned int nKeys = 256u;
		std::bitset<nKeys> keyStates;		// 按键状态
		std::queue<InputEvent> events;		// 输入事件队列
		std::queue<char> chars;				// 输入字符队列
	};
}