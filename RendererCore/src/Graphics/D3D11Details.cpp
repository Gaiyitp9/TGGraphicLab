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
		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));
		RetrieveHardwareInfo();
		CreateDeviceAndContext();
	}

	inline ID3D11Device* Device<LowLevelAPI::DirectX11>::operator->()
	{
		return device.Get();
	}

	void Device<LowLevelAPI::DirectX11>::GetDeviceContext(Context<LowLevelAPI::DirectX11>* pContext)
	{
		pContext->context.Swap(context);
	}

	std::vector<DXGI_MODE_DESC1> Device<LowLevelAPI::DirectX11>::GetOutputModes(Format format)
	{
		DXGI_FORMAT backBufferFormat = DirectXFormatMapping(format);

		ComPtr<IDXGIOutput1> pOutput1;
		adapterOutputs[0][0].As(&pOutput1);
		DXGI_OUTPUT_DESC outputDesc;
		pOutput1->GetDesc(&outputDesc);
		Debug::LogLine(std::format(L"Device name: {}", outputDesc.DeviceName));

		UINT modeCount = 0;
		pOutput1->GetDisplayModeList1(backBufferFormat, DXGI_ENUM_MODES_SCALING,
			&modeCount, nullptr);
		std::vector<DXGI_MODE_DESC1> outputModes(modeCount);
		if (modeCount > 0)
		{
			pOutput1->GetDisplayModeList1(backBufferFormat, DXGI_ENUM_MODES_SCALING,
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
		for (UINT i = 0; factory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			adapters.push_back(pAdapter);
			DXGI_ADAPTER_DESC desc;
			pAdapter->GetDesc(&desc);
			Debug::LogLine(std::format(L"Adpater vendor: {}. Video memory size: {} MB",
				desc.Description, desc.DedicatedVideoMemory / 1048576));

			std::vector<ComPtr<IDXGIOutput>> outputs;
			ComPtr<IDXGIOutput> pOutput;
			for (UINT j = 0; pAdapter->EnumOutputs(j, &pOutput) != DXGI_ERROR_NOT_FOUND; ++j)
				outputs.push_back(pOutput);
			adapterOutputs.push_back(outputs);
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
		D3D11CreateDevice(adapters[0].Get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr, creationFlags,
			featureLevels, 2, D3D11_SDK_VERSION, &device, nullptr, &context);
	}
}