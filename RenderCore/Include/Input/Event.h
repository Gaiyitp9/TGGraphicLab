/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <unordered_map>
#include <any>
#include "KeyCode.h"

namespace TG::Input
{
	struct EventInfo
	{
		static std::unordered_map<KeyCode, char const*> keysName;
	};
}
