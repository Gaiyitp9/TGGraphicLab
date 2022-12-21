/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "GraphicsBase.hpp"
#include <d3d11_4.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

namespace TG::Graphics
{
	DXGI_FORMAT DirectXFormatMapping(Format format);

	template<>
	class Device<LowLevelAPI::DirectX11>
	{
	public:
		Device();
		Device(const Device&) = delete;
		Device& operator=(const Device&) = delete;

	private:
		std::vector<DXGI_MODE_DESC1> GetOutputModes(DXGI_FORMAT format);
		void RetrieveHardwareInfo();
		void CreateDeviceAndContext();

	private:
		ComPtr<IDXGIFactory> dxgiFactory;
		ComPtr<ID3D11Device> d3dDevice;
		ComPtr<ID3D11DeviceContext> d3dContext;

		std::vector<ComPtr<IDXGIAdapter>> dxgiAdapters;
		std::vector<std::vector<ComPtr<IDXGIOutput>>> dxgiOutputs;

		friend class Context<LowLevelAPI::DirectX11>;
		friend class FrameBuffer<LowLevelAPI::DirectX11>;
	};

	template<>
	class Context<LowLevelAPI::DirectX11>
	{
	public:
		Context(const std::shared_ptr<Device<LowLevelAPI::DirectX11>>& device);
		Context(const Context&) = delete;
		Context& operator=(const Context&) = delete;

		void ClearFrameBuffer(const std::shared_ptr<FrameBuffer<LowLevelAPI::DirectX11>>& buffer, 
			const Math::Color& color);

	public:
		ComPtr<ID3D11DeviceContext> d3dContext;
	};

	template<>
	class FrameBuffer<LowLevelAPI::DirectX11>
	{
	public:
		FrameBuffer(const std::shared_ptr<Device<LowLevelAPI::DirectX11>>& device, 
			const std::shared_ptr<Window>& window);
		FrameBuffer(const FrameBuffer&) = delete;
		FrameBuffer& operator=(const FrameBuffer&) = delete;

		void Present();

	private:
		std::weak_ptr<Window> window;
		ComPtr<IDXGISwapChain> swapChain;
		ComPtr<ID3D11RenderTargetView> renderTargetView;
		std::vector<DXGI_MODE_DESC1> outputModes;

		DXGI_FORMAT backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		UINT sampleCount = 4;
		DXGI_RATIONAL refreshRate;
		UINT numQualityLevels;

		friend class Context<LowLevelAPI::DirectX11>;
	};

#ifdef _DEBUG
	template<>
	class DebugInfo<LowLevelAPI::DirectX11>
	{
	public:
		DebugInfo();
		~DebugInfo();
		DebugInfo(const DebugInfo&) = delete;
		DebugInfo& operator=(const DebugInfo&) = delete;

		void ReportLiveObjects();		// 输出所有d3d和dxgi物体
		void OutputMessages();			// 输出调试信息

	private:
		using DXGIGetDebugInterface = HRESULT(*)(REFIID, void**);
		HMODULE module;
		DXGIGetDebugInterface DxgiGetDebugInterface;

		ComPtr<IDXGIDebug1> dxgiDebug;
		ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
		unsigned long long next;						// 下一条消息的索引
	};
#endif
}