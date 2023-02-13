/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Graphics/Direct3D11/D3D11Factory.hpp"
#include "Diagnostics/Debug.hpp"

namespace TG::Graphics
{
	D3D11Factory::D3D11Factory()
	{
		CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory));
	}

	bool D3D11Factory::EnumAdapters(unsigned int index, AdapterDesc& adapterDesc) const
	{
		// 枚举所有显示适配器及其输出
		winrt::com_ptr<IDXGIAdapter> pAdapter;
		if (m_dxgiFactory->EnumAdapters(index, pAdapter.put()) == DXGI_ERROR_NOT_FOUND);
			return false;

		DXGI_ADAPTER_DESC desc;
		pAdapter->GetDesc(&desc);
		adapterDesc.description = desc.Description;
		adapterDesc.videoMemory = desc.DedicatedVideoMemory / 1048576;
		Debug::LogLine(std::format(L"Adpater vendor: {}. Video memory size: {} MB",
				adapterDesc.description, adapterDesc.videoMemory));
		return true;
	}
}
