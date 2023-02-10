/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Keyboard.hpp"
#include "Mouse.hpp"

namespace TG
{
	class InputSystem
	{
	public:
		InputSystem();
		InputSystem(const InputSystem&) = delete;
		InputSystem& operator=(const InputSystem&) = delete;
		~InputSystem();

		bool GetKey(KeyCode key) const;			// 是否按住按键
		bool GetKeyDown(KeyCode key) const;		// 是否按下按键
		bool GetKeyUp(KeyCode key) const;		// 是否释放按键

	private:
		void Update();							// 更新输入系统

	private:
		Keyboard keyboard;
		Mouse mouse;

		friend class MainWindow;
	};
}