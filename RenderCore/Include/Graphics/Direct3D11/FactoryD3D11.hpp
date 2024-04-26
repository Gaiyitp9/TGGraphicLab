/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once
#include <winrt/base.h>
#include <dxgi.h>
#include "../Factory.hpp"
#include "CreateInfoD3D11.hpp"
#include "RenderDeviceD3D11.hpp"
#include "DeviceContextD3D11.hpp"
#include "SwapChainD3D11.hpp"
#include "FormatMap.hpp"

namespace TG::Graphics
{
	class FactoryD3D11 final : public IFactory
	{
	public:
		FactoryD3D11();

		bool EnumAdapter(unsigned int index, AdapterDesc& desc) const override;
		void CreateDeviceAndContext(ICreateInfo const* info, IRenderDevice** ppDevice, IDeviceContext** ppContext) const override;
		void CreateSwapChain(IRenderDevice const* pDevice, HWND hwnd, const SwapChainDesc& desc, ISwapChain** ppSwapChain) const override;

	private:
		winrt::com_ptr<IDXGIFactory1> m_dxgiFactory;
	};
}
