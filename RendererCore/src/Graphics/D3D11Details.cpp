/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Graphics/D3D11Details.hpp"

namespace LCH::Graphics
{
	DXGI_FORMAT DirectXFormatMapping(Format format)
	{
		static std::unordered_map<Format, DXGI_FORMAT> map =
		{
			{Format::R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM},
		};
		return map[format];
	}

	Device<LowLevelAPI::DirectX11>::Device()
	{
		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)));
		RetrieveHardwareInfo();
		CreateDeviceAndContext();
	}

	inline ID3D11Device* Device<LowLevelAPI::DirectX11>::operator->()
	{
		return d3dDevice.Get();
	}

	std::vector<DXGI_MODE_DESC1> Device<LowLevelAPI::DirectX11>::GetOutputModes(DXGI_FORMAT format)
	{
		ComPtr<IDXGIOutput1> pOutput1;
		dxgiOutputs[0][0].As(&pOutput1);
		DXGI_OUTPUT_DESC outputDesc;
		pOutput1->GetDesc(&outputDesc);
		Debug::LogLine(std::format(L"Device name: {}", outputDesc.DeviceName));

		UINT modeCount = 0;
		pOutput1->GetDisplayModeList1(format, DXGI_ENUM_MODES_SCALING, &modeCount, nullptr);
		std::vector<DXGI_MODE_DESC1> outputModes(modeCount);
		if (modeCount > 0)
		{
			pOutput1->GetDisplayModeList1(format, DXGI_ENUM_MODES_SCALING,
				&modeCount, outputModes.data());
			for (UINT i = 0; i < modeCount; ++i)
			{
				Debug::LogLine(std::format(L"Width: {}  Height: {} Format: {} Refresh rate denominator: {} numerator: {}",
					outputModes[i].Width, outputModes[i].Height,
					(UINT)(outputModes[i].Format),
					outputModes[i].RefreshRate.Denominator,
					outputModes[i].RefreshRate.Numerator));
			}
		}
		return outputModes;
	}

	void Device<LowLevelAPI::DirectX11>::RetrieveHardwareInfo()
	{
		// 枚举所有显示适配器及其输出
		ComPtr<IDXGIAdapter> pAdapter;
		for (UINT i = 0; dxgiFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			dxgiAdapters.push_back(pAdapter);
			DXGI_ADAPTER_DESC desc;
			pAdapter->GetDesc(&desc);
			Debug::LogLine(std::format(L"Adpater vendor: {}. Video memory size: {} MB",
				desc.Description, desc.DedicatedVideoMemory / 1048576));

			std::vector<ComPtr<IDXGIOutput>> outputs;
			ComPtr<IDXGIOutput> pOutput;
			for (UINT j = 0; pAdapter->EnumOutputs(j, &pOutput) != DXGI_ERROR_NOT_FOUND; ++j)
				outputs.push_back(pOutput);
			dxgiOutputs.push_back(outputs);
		}
	}

	void Device<LowLevelAPI::DirectX11>::CreateDeviceAndContext()
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
		D3D11CreateDevice(dxgiAdapters[0].Get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr, creationFlags,
			featureLevels, 2, D3D11_SDK_VERSION, &d3dDevice, nullptr, &d3dContext);
	}

	Context<LowLevelAPI::DirectX11>::Context(Device<LowLevelAPI::DirectX11>* device)
	{
		d3dContext.Swap(device->d3dContext);
	}

	inline ID3D11DeviceContext* Context<LowLevelAPI::DirectX11>::operator->()
	{
		return d3dContext.Get();
	}

	void Context<LowLevelAPI::DirectX11>::ClearFrameBuffer(FrameBuffer<LowLevelAPI::DirectX11>* frameBuffer,
		float red, float green, float blue)
	{
		const float color[] = { red, green, blue, 1.0f };
		d3dContext->ClearRenderTargetView(frameBuffer->renderTargetView.Get(), color);
	}

	FrameBuffer<LowLevelAPI::DirectX11>::FrameBuffer(Device<LowLevelAPI::DirectX11>* device, Window const* window)
		: window(window)
	{
		if (window == nullptr)
			ThrowBaseExcept(L"Parameter window can not be nullptr");

		ThrowIfFailed(device->d3dDevice->CheckMultisampleQualityLevels(backBufferFormat, sampleCount,
			&numQualityLevels));

		outputModes = device->GetOutputModes(backBufferFormat);
		refreshRate = outputModes[0].RefreshRate;

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

		ThrowIfFailed(device->dxgiFactory->CreateSwapChain(device->d3dDevice.Get(), &swapChainDesc, &swapChain));

		ComPtr<ID3D11Resource> pBackBuffer;
		swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		device->d3dDevice->CreateRenderTargetView(
			pBackBuffer.Get(),
			nullptr,
			&renderTargetView
		);
	}

	void FrameBuffer<LowLevelAPI::DirectX11>::Present()
	{
		swapChain->Present(1u, 0u);
	}
}