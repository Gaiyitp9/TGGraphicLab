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
#include "pch.h"
#include "InputSystem.h"

namespace LCH
{
	InputSystem::InputSystem()
	{

	}

	InputSystem::~InputSystem()
	{

	}

	void InputSystem::OnKeyPressed(size_t key)
	{
		keyStates.set(key, true);
		events.push(InputEvent(static_cast<KeyCode>(key), InputEvent::EventType::Pressed));
	}

	void InputSystem::OnKeyReleased(size_t key)
	{
		keyStates.set(key, false);
		events.push(InputEvent(static_cast<KeyCode>(key), InputEvent::EventType::Released));
	}

	bool InputSystem::GetKey(KeyCode key) const noexcept
	{
		size_t index = static_cast<size_t>(key);
		return keyStates[index];
	}
}