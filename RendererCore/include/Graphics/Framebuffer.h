/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "../Window.h"
#include <d3d11_4.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

namespace LCH::Graphics
{
	class Framebuffer
	{
	public:
		Framebuffer(const Window&);
		~Framebuffer();

		void ClearRenderTarget(float red, float green, float blue);

	private:
		ComPtr<IDXGISwapChain> pSwapChain;
		ComPtr<ID3D11RenderTargetView> pRenderTargetView;
		DXGI_FORMAT backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		DXGI_RATIONAL refreshRate;
		UINT sampleCount = 4;
		UINT numQualityLevels;
	};
}