/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Diagnostics/MemoryDbg.h"
#include "Diagnostics/Debug.h"
#include "Graphics/D3D11Details.hpp"

namespace LCH::Graphics
{
	GraphicsLayer<LowLevelAPI::DirectX11>::GraphicsLayer(const Window& window)
		: window(window)
	{
		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&pFactory)));
		EnumerateAdapters();
		CreateDeviceAndSwapChain();
	}

	GraphicsLayer<LowLevelAPI::DirectX11>::~GraphicsLayer()
	{

	}

	void GraphicsLayer<LowLevelAPI::DirectX11>::EndFrame()
	{
		pSwapChain->Present(1u, 0u);
	}

	void GraphicsLayer<LowLevelAPI::DirectX11>::CreateFrameBuffer()
	{

	}

	void GraphicsLayer<LowLevelAPI::DirectX11>::EnumerateAdapters()
	{
		ComPtr<IDXGIAdapter1> pAdapter;
		for (UINT i = 0; pFactory->EnumAdapters1(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			adapters.push_back(pAdapter);
			adapterOutputs.push_back(std::vector<ComPtr<IDXGIOutput>>());
			ComPtr<IDXGIOutput> pOutput;
			for (UINT j = 0; pAdapter->EnumOutputs(j, &pOutput) != DXGI_ERROR_NOT_FOUND; ++j)
				adapterOutputs[i].push_back(pOutput);
		}

		for (UINT i = 0; i < adapters.size(); ++i)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapters[i]->GetDesc1(&desc);
			Debug::LogLine(std::format(L"Adpater vendor: {}. Video memory size: {} MB",
				desc.Description, desc.DedicatedVideoMemory / 1048576));
			for (UINT j = 0; j < adapterOutputs[i].size(); ++j)
			{
				ComPtr<IDXGIOutput1> pOutput1;
				adapterOutputs[i][j].As(&pOutput1);

				DXGI_OUTPUT_DESC outputDesc;
				pOutput1->GetDesc(&outputDesc);
				Debug::LogLine(std::format(L"Device name: {}", outputDesc.DeviceName));

				UINT modeCount = 0;
				pOutput1->GetDisplayModeList1(backBufferFormat, DXGI_ENUM_MODES_SCALING,
					&modeCount, nullptr);
				if (modeCount > 0)
				{
					DXGI_MODE_DESC1* outputModeDesc = new DXGI_MODE_DESC1[modeCount];
					pOutput1->GetDisplayModeList1(backBufferFormat, DXGI_ENUM_MODES_SCALING,
						&modeCount, outputModeDesc);
					for (UINT k = 0; k < modeCount; ++k)
					{
						Debug::LogLine(std::format(L"Width: {}  Height: {} Format: {} Refresh rate denominator: {} numerator: {}", 
							outputModeDesc[k].Width, outputModeDesc[k].Height, 
							(UINT)(outputModeDesc[k].Format),
							outputModeDesc[k].RefreshRate.Denominator,
							outputModeDesc[k].RefreshRate.Numerator));
					}
					refreshRate = outputModeDesc[0].RefreshRate;
					delete[] outputModeDesc;
				}
			}
		}
	}

	void GraphicsLayer<LowLevelAPI::DirectX11>::CreateDeviceAndSwapChain()
	{
		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};

		ThrowIfFailed(D3D11CreateDevice(adapters[0].Get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr,
			creationFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, 
			&pDevice, nullptr, &pContext));
		ThrowIfFailed(pDevice->CheckMultisampleQualityLevels(backBufferFormat, sampleCount, 
			&numQualityLevels));

		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferDesc.Width = window.Width();
		swapChainDesc.BufferDesc.Height = window.Height();
		swapChainDesc.BufferDesc.Format = backBufferFormat;
		swapChainDesc.BufferDesc.RefreshRate = refreshRate;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.SampleDesc.Count = sampleCount;
		swapChainDesc.SampleDesc.Quality = numQualityLevels - 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow = window.Hwnd();
		swapChainDesc.Windowed = true;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		ThrowIfFailed(pFactory->CreateSwapChain(pDevice.Get(), &swapChainDesc, &pSwapChain));

		ComPtr<ID3D11Resource> pBackBuffer;
		pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		pDevice->CreateRenderTargetView(
			pBackBuffer.Get(),
			nullptr,
			&pRenderTargetView
		);
	}

	void GraphicsLayer<LowLevelAPI::DirectX11>::ClearRenderTarget(float red, float green, float blue)
	{
		const float color[] = { red, green, blue, 1.0f };
		pContext->ClearRenderTargetView(pRenderTargetView.Get(), color);
	}
}