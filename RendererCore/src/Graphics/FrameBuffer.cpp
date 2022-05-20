/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Graphics/FrameBuffer.h"

namespace LCH::Graphics
{
	FrameBuffer::FrameBuffer(IDXGIFactory* factory, ID3D11Device* device, Window const* window)
	{
		ThrowIfFailed(device->CheckMultisampleQualityLevels(backBufferFormat, sampleCount,
			&numQualityLevels));

		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferDesc.Width = window->Width();
		swapChainDesc.BufferDesc.Height = window->Height();
		swapChainDesc.BufferDesc.Format = backBufferFormat;
		swapChainDesc.BufferDesc.RefreshRate = refreshRate;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.SampleDesc.Count = sampleCount;
		swapChainDesc.SampleDesc.Quality = numQualityLevels - 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow = window->Hwnd();
		swapChainDesc.Windowed = true;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		ThrowIfFailed(factory->CreateSwapChain(device, &swapChainDesc, &swapChain));

		ComPtr<ID3D11Resource> pBackBuffer;
		swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		device->CreateRenderTargetView(
			pBackBuffer.Get(),
			nullptr,
			&renderTargetView
		);
	}

	FrameBuffer::~FrameBuffer()
	{

	}
}