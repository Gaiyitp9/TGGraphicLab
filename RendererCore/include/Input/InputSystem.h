/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Keyboard.h"
#include "Mouse.h"

namespace LCH
{
	class InputSystem
	{
	public:
		InputSystem();
		InputSystem(const InputSystem&) = delete;
		InputSystem& operator=(const InputSystem&) = delete;
		~InputSystem();

		void Update();							// 更新输入系统

		bool GetKey(KeyCode key) const;			// 是否按住按键
		bool GetKeyDown(KeyCode key) const;		// 是否按下按键
		bool GetKeyUp(KeyCode key) const;		// 是否释放按键

		void SpyInputEvent() noexcept;			// 是否监控输入事件
		void StopSpyInputEvent() noexcept;		// 是否停止监控输入事件

	private:
		Keyboard keyboard;
		Mouse mouse;

		friend class Window;
	};
}