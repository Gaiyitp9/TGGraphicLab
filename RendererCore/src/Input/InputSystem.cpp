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

	bool InputSystem::GetKey(KeyCode key) const
	{
		return false;
	}

	bool InputSystem::GetKeyDown(KeyCode key) const
	{
		return keyboard.keyStates.test(static_cast<size_t>(key));
	}
}