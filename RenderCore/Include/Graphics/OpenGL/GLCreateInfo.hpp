/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once
#include "Win32Lean.h"
#include <cstdint>

namespace TG::Graphics
{
	struct GLCreateInfo
	{
        std::uint16_t adapterIndex{0};
        HWND hwnd{nullptr};
	};
}
