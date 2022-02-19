/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

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

	bool InputSystem::GetKey(KeyCode key) const
	{
		size_t index = static_cast<size_t>(key);
		return keyStates[index];
	}
}