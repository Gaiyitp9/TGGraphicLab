/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Graphics.hpp"

namespace LCH::Graphics
{
	template <LowLevelAPI API = LowLevelAPI::DirectX11>
	class GraphicsLayer
	{
	public:
		GraphicsLayer();

	public:
		Device<API> device;
		Context<API> context;
		FrameBuffer<API> frameBuffer;
	};
}