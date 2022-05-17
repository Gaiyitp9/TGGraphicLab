/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Graphics.h"
#include <d3d11_4.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

namespace LCH::Graphics
{
	class D3D11Layer : public IGraphics
	{
	public:
		D3D11Layer(const Window& window);
		D3D11Layer(D3D11Layer&) = delete;
		D3D11Layer& operator=(const D3D11Layer&) = delete;
		~D3D11Layer();

		void ClearRenderTarget(float red, float green, float blue);
		void EndFrame();

	private:
		void EnumerateAdapters();
		void CreateDeviceAndSwapChain();

	private:
		ComPtr<ID3D11Device> pDevice;
		ComPtr<ID3D11DeviceContext> pContext;
		ComPtr<IDXGIFactory1> pFactory;
		ComPtr<ID3D11RenderTargetView> pRenderTargetView;
		ComPtr<IDXGISwapChain> pSwapChain;

	private:
		const Window& window;
		std::vector<ComPtr<IDXGIAdapter1>> adapters;
		std::vector<std::vector<ComPtr<IDXGIOutput>>> adapterOutputs;
		DXGI_FORMAT backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		DXGI_RATIONAL refreshRate;
		UINT sampleCount = 4;
		UINT numQualityLevels;
	};
}