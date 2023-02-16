/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once
#include "GraphicsBase.hpp"
#include "RenderDevice.hpp"
#include "DeviceContext.hpp"
#include "SwapChain.hpp"

namespace TG::Graphics
{
	struct IFactory
	{
		// 枚举显示适配器，如果对应索引存在，则返回true，否则返回false
		virtual bool EnumAdapter(unsigned int index, AdapterDesc& desc) const = 0;

		// 创建图形设备和上下文
		virtual void CreateDeviceAndContext(ICreateInfo const* info, IRenderDevice** ppDevice, IDeviceContext** ppContext) const = 0;

		// 创建交换链
		virtual void CreateSwapChain(IRenderDevice const* pDevice, IDeviceContext const* pContext, const SwapChainDesc& desc, ISwapChain** ppSwapChain) const = 0;
	};
}
