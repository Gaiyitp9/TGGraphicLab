/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Graphics/Graphics.hpp"

namespace LCH::Graphics
{
	template <LowLevelAPI API>
	class GraphicsLayer
	{
	public:
		GraphicsLayer()
		{
			device = std::make_unique<Device<API>>();
			context = std::make_unique<Context<API>>(device.get());
		}

		void Update()
		{
			for (const auto& frameBuffer : frameBuffers)
			{
				frameBuffer.second->Present();
			}
		}

		bool CreateFrameBuffer(Window const* window)
		{
			if (!frameBuffers.contains(window))
			{
				frameBuffers[window] = std::make_unique<FrameBuffer<API>>(device.get(), window);
				return true;
			}
			return false;
		}

		void ClearBackground(Window const* window, const Math::Color& color)
		{
			context->ClearFrameBuffer(frameBuffers[window].get(), color);
		}

	public:
		std::unique_ptr<Device<API>> device;
		std::unique_ptr<Context<API>> context;
		std::unordered_map<Window const*, std::unique_ptr<FrameBuffer<API>>> frameBuffers;
	};
}