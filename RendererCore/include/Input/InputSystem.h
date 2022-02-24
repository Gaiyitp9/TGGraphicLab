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

		void Update();

		bool GetKey(KeyCode key) const;
		bool GetKeyDown(KeyCode key) const;
		bool GetKeyUp(KeyCode key) const;

	public:
		Keyboard keyboard;
		Mouse mouse;
	};
}