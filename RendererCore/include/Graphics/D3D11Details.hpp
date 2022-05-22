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
	DXGI_FORMAT DirectXFormatMapping(Format format);

	template<>
	class Device<LowLevelAPI::DirectX11>
	{
	public:
		Device();
		ID3D11Device* operator->();
		void GetDeviceContext(Context<LowLevelAPI::DirectX11>* pContext);
		std::vector<DXGI_MODE_DESC1> GetOutputModes(Format format);

	private:
		void RetrieveHardwareInfo();
		void CreateDeviceAndContext();

	private:
		ComPtr<IDXGIFactory> factory;
		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> context;
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
		friend void Device<LowLevelAPI::DirectX11>::GetDeviceContext(Context<LowLevelAPI::DirectX11>* pContext);
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