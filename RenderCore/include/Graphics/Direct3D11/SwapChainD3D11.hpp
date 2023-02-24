/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once
#include "../SwapChain.hpp"
#include <winrt/base.h>
#include <d3d11.h>

namespace TG::Graphics
{
	class SwapChainD3D11 : public ISwapChain
	{
	public:
		SwapChainD3D11(ID3D11Device* device,
			const winrt::com_ptr<IDXGISwapChain>& swapChain);

		virtual void Present(int syncInterval);
		virtual void Resize(int width, int height) {}

		ID3D11RenderTargetView* GetCurrentBackBufferRTV();

	private:
		winrt::com_ptr<IDXGISwapChain> m_swapChain;
		winrt::com_ptr<ID3D11RenderTargetView> m_renderTargetView;
	};
}
