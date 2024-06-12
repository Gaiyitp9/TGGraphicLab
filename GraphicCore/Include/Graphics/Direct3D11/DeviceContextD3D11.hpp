/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once
#include "../DeviceContext.hpp"
#include "CreateInfoD3D11.hpp"
#include <winrt/base.h>
#include <d3d11.h>

namespace TG::Graphics
{
	class DeviceContextD3D11 : public IDeviceContext
	{
	public:
		DeviceContextD3D11(const CreateInfoD3D11& info, const winrt::com_ptr<ID3D11DeviceContext>& context);
		DeviceContextD3D11(const DeviceContextD3D11&) = delete;
		DeviceContextD3D11(const DeviceContextD3D11&&) = delete;
		DeviceContextD3D11& operator=(const DeviceContextD3D11&) = delete;

		virtual void SetPipelineState() {}
		virtual void SetVertexBuffers() {}
		virtual void SetIndexBuffer() {}
		virtual void SetViewports() {}
		virtual void SetRenderTargets() {}
		virtual void Draw() {}
		virtual void DrawIndexed() {}

	//private:
	public:
		winrt::com_ptr<ID3D11DeviceContext> m_context;
	};
}
