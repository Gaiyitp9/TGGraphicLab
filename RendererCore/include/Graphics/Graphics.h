/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "../Window.h"

namespace LCH::Graphics
{
	enum class GraphicAPI
	{
		DirectX11,
		DirectX12,
		OpenGL,
		Vulcan,
	};

	template <GraphicAPI API>
	class IGraphics
	{
	public:
		//virtual void Device() = 0;
		//virtual void Context() = 0;
	};
}