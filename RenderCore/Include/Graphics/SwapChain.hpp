/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once
#include "Diagnostics/Log.hpp"
#include "Diagnostics/Win32Exception.h"

namespace TG::Graphics
{
	struct ISwapChain
	{
		virtual void Present(int syncInterval) = 0;
		virtual void Resize(int width, int height) = 0;
	};
}