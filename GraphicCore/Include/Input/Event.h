/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "KeyCode.h"

namespace TG::Input
{
	class Mouse;
	class Keyboard;

	template<typename Device> struct Event;

	template<>
	struct Event<Mouse>
	{
		KeyCode key{ KeyCode::None };
		bool isPressed{ false };
		short x{ 0 };
		short y{ 0 };
		short wheelDelta{ 0 };
	};

	template<>
	struct Event<Keyboard>
	{
		KeyCode key{ KeyCode::None };
		bool isPressed{ false };
		char16_t c{ '\0' };		// 字符的Unicode码，只支持基本多文种平面(BMP)
	};
}
