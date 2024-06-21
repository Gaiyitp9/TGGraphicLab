/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Graphics/Direct3D11/RenderDeviceD3D11.hpp"

namespace TG::Graphics
{
	RenderDeviceD3D11::RenderDeviceD3D11(const CreateInfoD3D11& info,
		const winrt::com_ptr<ID3D11Device>& pDevice,
		const winrt::com_ptr<IDXGIAdapter>& pAdapter)
	{
		m_device = pDevice;
		m_adapter = pAdapter;
	}

	ID3D11Device* RenderDeviceD3D11::device() const
	{
		return m_device.get();
	}

	IDXGIAdapter* RenderDeviceD3D11::adapter() const
	{
		return m_adapter.get();
	}
}
