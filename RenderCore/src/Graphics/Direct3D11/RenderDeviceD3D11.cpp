/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Graphics/Direct3D11/RenderDeviceD3D11.hpp"

namespace TG::Graphics
{
	RenderDeviceD3D11::RenderDeviceD3D11(const D3D11CreateInfo& info)
	{
		CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory));
	}
}
