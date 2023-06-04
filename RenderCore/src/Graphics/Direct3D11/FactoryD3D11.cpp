/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Graphics/Direct3D11/FactoryD3D11.hpp"
#include "Diagnostics/Log.hpp"

namespace TG::Graphics
{
	FactoryD3D11::FactoryD3D11()
	{
		CheckHResult(CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory)));
	}

	bool FactoryD3D11::EnumAdapter(unsigned int index, AdapterDesc& adapterDesc) const
	{
		winrt::com_ptr<IDXGIAdapter> pAdapter;
		if (m_dxgiFactory->EnumAdapters(index, pAdapter.put()) == DXGI_ERROR_NOT_FOUND);
			return false;

		DXGI_ADAPTER_DESC desc;
		CheckHResult(pAdapter->GetDesc(&desc));
		adapterDesc.description = desc.Description;
		adapterDesc.videoMemory = desc.DedicatedVideoMemory / 1048576;
		return true;
	}

	void FactoryD3D11::CreateDeviceAndContext(ICreateInfo const* info, IRenderDevice** ppDevice, IDeviceContext** ppContext) const
	{
		CreateInfoD3D11 const* d3dInfo = static_cast<CreateInfoD3D11 const*>(info);

		winrt::com_ptr<ID3D11Device> d3dDevice;
		winrt::com_ptr<ID3D11DeviceContext> d3dContext;
		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};
		winrt::com_ptr<IDXGIAdapter> pAdapter;
		if (m_dxgiFactory->EnumAdapters(d3dInfo->adapterIndex, pAdapter.put()) == DXGI_ERROR_NOT_FOUND)
			throw BaseException(L"Invalid adapter index");

		CheckHResult(D3D11CreateDevice(pAdapter.get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr, creationFlags,
			featureLevels, 2, D3D11_SDK_VERSION, d3dDevice.put(), nullptr, d3dContext.put()));

		*ppDevice = DBG_NEW RenderDeviceD3D11(*d3dInfo, d3dDevice, pAdapter);
		*ppContext = DBG_NEW DeviceContextD3D11(*d3dInfo, d3dContext);
	}

	void FactoryD3D11::CreateSwapChain(IRenderDevice const* pDevice, HWND hwnd, const SwapChainDesc& desc, ISwapChain** ppSwapChain) const
	{
		RenderDeviceD3D11 const* d3dDevice = dynamic_cast<RenderDeviceD3D11 const*>(pDevice);
		if (d3dDevice == nullptr)
			throw BaseException(L"pDevice should be RenderDeviceD3D11 type");

		DXGI_FORMAT format = TexFormatToDXGIFormat(desc.format);

		// 获取第一个显示输出
		winrt::com_ptr<IDXGIOutput> pOutput;
		if (d3dDevice->adapter()->EnumOutputs(0, pOutput.put()) == DXGI_ERROR_NOT_FOUND)
			throw BaseException(L"No display exist");
		// 根据格式获取显示模式
		UINT modeCount = 0;
		CheckHResult(pOutput->GetDisplayModeList(format, DXGI_ENUM_MODES_SCALING, &modeCount, nullptr));
		std::vector<DXGI_MODE_DESC> outputModes(modeCount);
		if (modeCount > 0)
		{
			CheckHResult(pOutput->GetDisplayModeList(format, DXGI_ENUM_MODES_SCALING,
				&modeCount, outputModes.data()));
		}

		// 获取多重采样级别
		UINT numQualityLevels;
		CheckHResult(d3dDevice->device()->CheckMultisampleQualityLevels(format, desc.sampleCount,
			&numQualityLevels));

		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferDesc.Width = desc.width;
		swapChainDesc.BufferDesc.Height = desc.height;
		swapChainDesc.BufferDesc.Format = format;
		swapChainDesc.BufferDesc.RefreshRate = outputModes[0].RefreshRate;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.SampleDesc.Count = desc.sampleCount;
		swapChainDesc.SampleDesc.Quality = numQualityLevels - 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = desc.bufferCount;
		swapChainDesc.OutputWindow = hwnd;
		swapChainDesc.Windowed = true;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		winrt::com_ptr<IDXGISwapChain> swapChain;
		CheckHResult(m_dxgiFactory->CreateSwapChain(d3dDevice->device(), &swapChainDesc, swapChain.put()));
		
		*ppSwapChain = DBG_NEW SwapChainD3D11(d3dDevice->device(), swapChain);
	}
}
