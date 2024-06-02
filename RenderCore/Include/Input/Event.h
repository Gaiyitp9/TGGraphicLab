/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "KeyCode.h"
#include <any>

namespace TG::Input
{
	enum class EventType : unsigned char
	{
		Character,
		Keyboard,
		Mouse,
	};

	struct KeyboardState
	{
		KeyCode key;
		bool isPressed;
	};

	struct MouseState
	{
		KeyCode button;
		bool isPressed;
		short x, y;
		short wheelDelta;
	};

	struct Event
	{
		EventType type;
		std::any state;
	};
}
