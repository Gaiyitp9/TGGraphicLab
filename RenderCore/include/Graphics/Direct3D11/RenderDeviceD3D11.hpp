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
		RenderDeviceD3D11(const D3D11CreateInfo& info, 
			const winrt::com_ptr<ID3D11Device>& pDevice,
			const winrt::com_ptr<IDXGIAdapter>& pAdapter);
		RenderDeviceD3D11(const RenderDeviceD3D11&) = delete;
		RenderDeviceD3D11(const RenderDeviceD3D11&&) = delete;
		RenderDeviceD3D11& operator=(const RenderDeviceD3D11&) = delete;

		virtual void CreateBuffer() override;
		virtual void CreateShader() override;
		virtual void CreateTexture() override;

		ID3D11Device* device() const;
		IDXGIAdapter* adapter() const;

	private:
		winrt::com_ptr<ID3D11Device> m_device;
		winrt::com_ptr<IDXGIAdapter> m_adapter;
	};
}
