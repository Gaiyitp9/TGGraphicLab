/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "../Window.h"

namespace LCH::Graphics
{
	class IGraphics
	{
	public:
		virtual void Initialize() = 0;
	};
}