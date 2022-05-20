/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Graphics.hpp"
#include <d3d11_4.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;
#include "GraphicsLayer.hpp"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

namespace LCH::Graphics
{
	template<>
	class Device<LowLevelAPI::DirectX11>
	{
	public:
		Device()
		{
			ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));
			RetrieveHardwareInfo();
		}

		ID3D11Device* operator->()
		{
			return device.Get();
		}

		DXGI_FORMAT DirectXFormatMapping(Format format)
		{
			static std::unordered_map<Format, DXGI_FORMAT> map =
			{
				{Format::R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM},
			};
			return map[format];
		}

		std::vector<DXGI_MODE_DESC1> GetOutputModes(Format format, UINT adapterIndex = 0, UINT outputIndex = 0)
		{
			DXGI_FORMAT backBufferFormat = DirectXFormatMapping(format);

			ComPtr<IDXGIOutput1> pOutput1;
			adapterOutputs[adapterIndex][outputIndex].As(&pOutput1);
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

	private:
		void RetrieveHardwareInfo()
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

	public:
		ComPtr<ID3D11Device> device;
		ComPtr<IDXGIFactory> factory;
		std::vector<ComPtr<IDXGIAdapter>> adapters;
		std::vector<std::vector<ComPtr<IDXGIOutput>>> adapterOutputs;
	};

	template<>
	class Context<LowLevelAPI::DirectX11>
	{
	public:
		ID3D11DeviceContext* operator->()
		{
			return context.Get();
		}

	private:
		ComPtr<ID3D11DeviceContext> context;
	};

	template<>
	class FrameBuffer<LowLevelAPI::DirectX11>
	{
	public:
		FrameBuffer(IDXGIFactory* factory, ID3D11Device* device, Window const*);
		~FrameBuffer();

	private:
		ComPtr<IDXGISwapChain> swapChain;
		ComPtr<ID3D11RenderTargetView> renderTargetView;
		std::vector<DXGI_MODE_DESC1> outputModes;

		DXGI_FORMAT backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		DXGI_RATIONAL refreshRate;
		UINT sampleCount = 4;
		UINT numQualityLevels;
	};

	template<>
	class GraphicsLayer<LowLevelAPI::DirectX11>
	{
	public:
		GraphicsLayer(const Window& window);
		GraphicsLayer(GraphicsLayer&) = delete;
		GraphicsLayer& operator=(const GraphicsLayer&) = delete;
		~GraphicsLayer();

		void ClearRenderTarget(float red, float green, float blue);
		void EndFrame();

		void CreateFrameBuffer();

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
		//std::vector<FrameBuffer> frameBuffers;
		DXGI_FORMAT backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		DXGI_RATIONAL refreshRate;
		UINT sampleCount = 4;
		UINT numQualityLevels;
	};
}