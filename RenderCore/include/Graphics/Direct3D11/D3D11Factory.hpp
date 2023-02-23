/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once
#include <winrt/base.h>
#include <dxgi.h>
#include "../Factory.hpp"
#include "D3D11CreateInfo.hpp"
#include "RenderDeviceD3D11.hpp"
#include "DeviceContextD3D11.hpp"
#include "SwapChainD3D11.hpp"
#include "FormatMap.hpp"

namespace TG::Graphics
{
	class D3D11Factory final : public IFactory
	{
	public:
		D3D11Factory();

		virtual bool EnumAdapter(unsigned int index, AdapterDesc& desc) const override;
		virtual void CreateDeviceAndContext(ICreateInfo const* info, IRenderDevice** ppDevice, IDeviceContext** ppContext) const override;
		virtual void CreateSwapChain(IRenderDevice const* pDevice,  HWND hwnd, const SwapChainDesc& desc, ISwapChain** ppSwapChain) const override;

	private:
		winrt::com_ptr<IDXGIFactory1> m_dxgiFactory;
	};
}
