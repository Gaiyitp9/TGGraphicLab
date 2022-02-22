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

		bool GetKey(KeyCode key) const;
		bool GetKeyDown(KeyCode key) const;
		bool GetKeyUp(KeyCode key) const;

		void OnKeyPressed(size_t key);
		void OnKeyReleased(size_t key);

		void OnMouseMove(int x, int y);
		
	private:
		Keyboard keyboard;
		Mouse mouse;
	};
}