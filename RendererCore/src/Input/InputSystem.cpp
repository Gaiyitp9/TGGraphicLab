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
	}

	bool InputSystem::GetKey(KeyCode key) const
	{
		return keyboard.keyStates.test(static_cast<size_t>(key));
	}

	bool InputSystem::GetKeyDown(KeyCode key) const
	{
		return keyboard.keyDown.test(static_cast<size_t>(key));
	}

	bool InputSystem::GetKeyUp(KeyCode key) const
	{
		return keyboard.keyUp.test(static_cast<size_t>(key));
	}
}