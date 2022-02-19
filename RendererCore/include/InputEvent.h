/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "KeyCode.h"

namespace LCH
{
	struct InputEvent
	{
	public:
		enum class EventType
		{
			Pressed,
			Released,
		};

	public:
		InputEvent(KeyCode key, EventType type);
		~InputEvent();

	public:
		KeyCode key;
		EventType type;
	};
}