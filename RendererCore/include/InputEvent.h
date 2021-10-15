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