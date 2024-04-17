/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Graphics/Core.hpp"
#include "Graphics/Direct3D11/FactoryD3D11.hpp"

namespace TG::Graphics
{
	class GraphicsLayer
	{
	public:
		GraphicsLayer(Window const* window)
		{
			factory = std::make_unique<FactoryD3D11>();
			CreateInfoD3D11 info;
			info.adapterIndex = 0;
			info.hwnd = window->Hwnd();

			IRenderDevice* pDevice = nullptr;
			IDeviceContext* pContext = nullptr;
			factory->CreateDeviceAndContext(&info, &pDevice, &pContext);
			device.reset(pDevice);
			context.reset(pContext);

			SwapChainDesc desc;
			desc.format = TextureFormat::R8G8B8A8_UNORM;
			desc.bufferCount = 2;
			desc.sampleCount = 4;
			desc.width = window->Width();
			desc.height = window->Height();
			ISwapChain* pSwapChain = nullptr;
			factory->CreateSwapChain(pDevice, window->Hwnd(), desc, &pSwapChain);
			swapChain.reset(pSwapChain);
		}

		void Update()
		{
			swapChain->Present(1);
		}

		void ClearBackground(const Math::Color& color)
		{
			DeviceContextD3D11* c = static_cast<DeviceContextD3D11*>(context.get());
			SwapChainD3D11* s = static_cast<SwapChainD3D11*>(swapChain.get());
			c->m_context->ClearRenderTargetView(s->GetCurrentBackBufferRTV(), color.RGBA());
		}

	public:

#ifdef _DEBUG
		std::unique_ptr<DebugInfo<DeviceType::DirectX11>> dbgInfo;
#endif
		std::unique_ptr<IFactory> factory;
		std::unique_ptr<IRenderDevice> device;
		std::unique_ptr<IDeviceContext> context;
		std::unique_ptr<ISwapChain> swapChain;
	};
}