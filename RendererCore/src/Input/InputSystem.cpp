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
		return keyboard.keyStates.test(pos) || mouse.mouseStates.test(pos);
	}

	bool InputSystem::GetKeyDown(KeyCode key) const
	{
		size_t pos = static_cast<size_t>(key);
		return keyboard.keyDown.test(pos) || mouse.mouseDown.test(pos);
	}

	bool InputSystem::GetKeyUp(KeyCode key) const
	{
		size_t pos = static_cast<size_t>(key);
		return keyboard.keyUp.test(pos) || mouse.mouseUp.test(pos);
	}

	void InputSystem::SpyInputEvent() noexcept
	{
		keyboard.spyKeyboard = true;
		mouse.spyMouse = true;
	}

	void InputSystem::StopSpyInputEvent() noexcept
	{
		keyboard.spyKeyboard = false;
		mouse.spyMouse = false;
	}
}