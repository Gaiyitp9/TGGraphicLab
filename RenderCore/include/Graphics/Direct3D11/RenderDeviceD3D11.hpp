/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once
#include "../RenderDevice.hpp"
#include "D3D11CreateInfo.hpp"
#include <winrt/base.h>
#include <d3d11.h>

namespace TG::Graphics
{
	class RenderDeviceD3D11 final : public IRenderDevice
	{
	public:
		RenderDeviceD3D11(const D3D11CreateInfo& info);

	private:
		void RetrieveAdaptersInfo();		// 检索所有显示适配器信息

	private:
		winrt::com_ptr<IDXGIFactory1> m_dxgiFactory;
		winrt::com_ptr<ID3D11Device> m_d3dDevice;
	};
}
