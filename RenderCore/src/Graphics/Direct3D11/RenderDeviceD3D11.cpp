/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Graphics/Direct3D11/RenderDeviceD3D11.hpp"

namespace TG::Graphics
{
	RenderDeviceD3D11::RenderDeviceD3D11(const D3D11CreateInfo& info, 
		const winrt::com_ptr<ID3D11Device>& pDevice,
		const winrt::com_ptr<IDXGIAdapter>& pAdapter)
	{
		m_device = pDevice;
		m_adapter = pAdapter;
	}

	inline ID3D11Device* RenderDeviceD3D11::device() const
	{
		return m_device.get();
	}

	inline IDXGIAdapter* RenderDeviceD3D11::adapter() const
	{
		return m_adapter.get();
	}
}
