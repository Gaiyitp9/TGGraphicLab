/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Graphics/D3D11Details.hpp"
#include <format>

namespace TG::Graphics
{
	DXGI_FORMAT DirectXFormatMapping(Format format)
	{
		static std::unordered_map<Format, DXGI_FORMAT> map =
		{
			{Format::R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM},
		};
		return map[format];
	}

	Device<DeviceType::DirectX11>::Device()
	{
		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)));
		RetrieveHardwareInfo();
		CreateDeviceAndContext();
	}

	std::vector<DXGI_MODE_DESC1> Device<DeviceType::DirectX11>::GetOutputModes(DXGI_FORMAT format)
	{
		ComPtr<IDXGIOutput1> pOutput1;
		ThrowIfFailed(dxgiOutputs[0][0].As(&pOutput1));
		DXGI_OUTPUT_DESC outputDesc;
		ThrowIfFailed(pOutput1->GetDesc(&outputDesc));
		Debug::LogLine(std::format(L"Device name: {}", outputDesc.DeviceName));

		UINT modeCount = 0;
		ThrowIfFailed(pOutput1->GetDisplayModeList1(format, DXGI_ENUM_MODES_SCALING, &modeCount, nullptr));
		std::vector<DXGI_MODE_DESC1> outputModes(modeCount);
		if (modeCount > 0)
		{
			ThrowIfFailed(pOutput1->GetDisplayModeList1(format, DXGI_ENUM_MODES_SCALING,
				&modeCount, outputModes.data()));
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

	void Device<DeviceType::DirectX11>::RetrieveHardwareInfo()
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

	void Device<DeviceType::DirectX11>::CreateDeviceAndContext()
	{
		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};
		ThrowIfFailed(D3D11CreateDevice(dxgiAdapters[0].Get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr, creationFlags,
			featureLevels, 2, D3D11_SDK_VERSION, &d3dDevice, nullptr, &d3dContext));
	}

	Context<DeviceType::DirectX11>::Context(const std::shared_ptr<Device<DeviceType::DirectX11>>& device)
	{
		if (!device)
			ThrowBaseExcept(L"'device' can not be null");

		d3dContext.Swap(device->d3dContext);
	}

	void Context<DeviceType::DirectX11>::ClearFrameBuffer(const std::shared_ptr<FrameBuffer<DeviceType::DirectX11>>& frameBuffer,
		const Math::Color& color)
	{
		if (frameBuffer)
			d3dContext->ClearRenderTargetView(frameBuffer->renderTargetView.Get(), color.RGBA());
	}

	FrameBuffer<DeviceType::DirectX11>::FrameBuffer(const std::shared_ptr<Device<DeviceType::DirectX11>>& device,
		const std::shared_ptr<Window>& window)
		: window(window)
	{
		if (!device || !window)
			ThrowBaseExcept(L"'device' and/or 'window' can not be null");

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
		ThrowIfFailed(swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)));
		ThrowIfFailed(device->d3dDevice->CreateRenderTargetView(
			pBackBuffer.Get(),
			nullptr,
			&renderTargetView
		));
	}

	void FrameBuffer<DeviceType::DirectX11>::Present()
	{
		ThrowIfFailed(swapChain->Present(1u, 0u));
	}

#ifdef _DEBUG
	DebugInfo<DeviceType::DirectX11>::DebugInfo()
	{
		module = GetModuleHandleW(L"dxgidebug.dll");
		if (nullptr == module)
			ThrowLastErrorWithDesc(L"Can not load dxgidebug.dll");

		DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
			GetProcAddress(module, "DXGIGetDebugInterface"));
		if (nullptr == DxgiGetDebugInterface)
			ThrowLastErrorWithDesc(L"Can not find DXGIGetDebugInterface function procedure address");

		ThrowIfFailed(DxgiGetDebugInterface(IID_PPV_ARGS(&dxgiDebug)));
		ThrowIfFailed(DxgiGetDebugInterface(IID_PPV_ARGS(&dxgiInfoQueue)));

		next = 0;
	}

	DebugInfo<DeviceType::DirectX11>::~DebugInfo()
	{
		const auto end = dxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
		while (next < end)
		{
			HRESULT hr;
			SIZE_T messageLength;
			hr = dxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, next, nullptr, &messageLength);
			if (hr == S_OK)
			{
				DXGI_INFO_QUEUE_MESSAGE* pMessage = (DXGI_INFO_QUEUE_MESSAGE*)malloc(messageLength);
				hr = dxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, next, pMessage, &messageLength);
				if (hr == S_OK)
					Debug::Log(std::format("{}\n", pMessage->pDescription));
				free(pMessage);
			}

			next++;
		}
	}

	void DebugInfo<DeviceType::DirectX11>::ReportLiveObjects()
	{
		dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
	}

	void DebugInfo<DeviceType::DirectX11>::OutputMessages()
	{
		const auto end = dxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
		while (next < end)
		{
			SIZE_T messageLength;
			ThrowIfFailed(dxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, next, nullptr, &messageLength));
			DXGI_INFO_QUEUE_MESSAGE* pMessage = (DXGI_INFO_QUEUE_MESSAGE*)malloc(messageLength);
			ThrowIfFailed(dxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, next++, pMessage, &messageLength));
			Debug::Log(std::format("{}\n", pMessage->pDescription));
			free(pMessage);
		}
	}
#endif
}