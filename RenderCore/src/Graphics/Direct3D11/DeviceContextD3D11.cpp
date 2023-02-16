/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Graphics/Direct3D11/DeviceContextD3D11.hpp"

namespace TG::Graphics
{
	DeviceContextD3D11::DeviceContextD3D11(const D3D11CreateInfo& info, const winrt::com_ptr<ID3D11DeviceContext>& context)
	{
		m_d3dContext = context;
	}

	HWND DeviceContextD3D11::hwnd() const
	{
		return m_hwnd;
	}
}
