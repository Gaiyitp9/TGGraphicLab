/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Input/InputSystem.h"

namespace LCH
{
	InputSystem::InputSystem()
	{

	}

	InputSystem::~InputSystem()
	{

	}

	void InputSystem::Update()
	{
		keyboard.Update();
		mouse.Update();
	}

	bool InputSystem::GetKey(KeyCode key) const
	{
		size_t pos = static_cast<size_t>(key);
		if (Keyboard::IsKeyboardCode(key))
			return keyboard.keyStates.test(pos);
		else if (Mouse::IsMouseCode(key))
			return mouse.mouseStates.test(pos);

		return false;
	}

	bool InputSystem::GetKeyDown(KeyCode key) const
	{
		size_t pos = static_cast<size_t>(key);
		if (Keyboard::IsKeyboardCode(key))
			return keyboard.keyDown.test(pos);
		else if (Mouse::IsMouseCode(key))
			return mouse.mouseDown.test(pos);

		return false;
	}

	bool InputSystem::GetKeyUp(KeyCode key) const
	{
		size_t pos = static_cast<size_t>(key);
		if (Keyboard::IsKeyboardCode(key))
			return keyboard.keyUp.test(pos);
		else if (Mouse::IsMouseCode(key))
			return mouse.mouseUp.test(pos);

		return false;
	}
}