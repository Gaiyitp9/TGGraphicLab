/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Graphics/Graphics.hpp"

namespace TG::Graphics
{
	template <DeviceType Type>
	class GraphicsLayer
	{
	public:
		GraphicsLayer()
		{
			device = std::make_shared<Device<Type>>();
			context = std::make_shared<Context<Type>>(device);
#ifdef _DEBUG
			dbgInfo = std::make_unique<DebugInfo<Type>>();
#endif
		}

		void Update()
		{
			auto it = frameBuffers.begin();
			while (it != frameBuffers.end())
			{
				// 如果Windows窗口被销毁，则移除对应的帧缓存
				if (it->first->Destroy())
					it = frameBuffers.erase(it);
				else
				{
					it->second->Present();
					++it;
				}
			}
#ifdef _DEBUG
			dbgInfo->OutputMessages();
#endif
		}

		bool CreateFrameBuffer(const std::shared_ptr<Window>& window)
		{
			if (!frameBuffers.contains(window))
			{
				frameBuffers[window] = std::make_shared<FrameBuffer<Type>>(device, window);
				return true;
			}
			return false;
		}

		void ClearBackground(const std::shared_ptr<Window>& window, const Math::Color& color)
		{
			if (frameBuffers.contains(window))
				context->ClearFrameBuffer(frameBuffers[window], color);
		}

	public:
		std::shared_ptr<Device<Type>> device;
		std::shared_ptr<Context<Type>> context;
		std::unordered_map<std::shared_ptr<Window>, std::shared_ptr<FrameBuffer<Type>>> frameBuffers;
#ifdef _DEBUG
		std::unique_ptr<DebugInfo<Type>> dbgInfo;
#endif
	};
}