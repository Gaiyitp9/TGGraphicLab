/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "KeyCode.h"

namespace TG::Input
{
	struct Event
	{
		virtual ~Event() = default;
	};

	struct CharacterEvent final : Event
	{
		// 字符的Unicode码，只支持基本多文种平面(BMP)
		char16_t c{ 0 };
	};

	struct KeyboardEvent final : Event
	{
		KeyCode key{ KeyCode::None };
		bool isPressed{ false };
	};

	struct MouseEvent final : Event
	{
		KeyCode button{ KeyCode::None };
		bool isPressed{ false };
		short x{ 0 }, y{ 0 };
		short wheelDelta{ 0 };
	};
}
