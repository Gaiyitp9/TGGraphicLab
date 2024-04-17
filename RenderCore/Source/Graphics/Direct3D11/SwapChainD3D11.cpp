/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Graphics/Direct3D11/SwapChainD3D11.hpp"

namespace TG::Graphics
{
	SwapChainD3D11::SwapChainD3D11(ID3D11Device* device,
		const winrt::com_ptr<IDXGISwapChain>& swapChain)
		: m_swapChain(swapChain)
	{
		winrt::com_ptr<ID3D11Resource> pBackBuffer;
		CheckHResult(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)));
		CheckHResult(device->CreateRenderTargetView(
			pBackBuffer.get(),
			nullptr,
			m_renderTargetView.put()
		));
	}

	void SwapChainD3D11::Present(int syncInterval)
	{
		m_swapChain->Present(syncInterval, 0);
	}

	ID3D11RenderTargetView* SwapChainD3D11::GetCurrentBackBufferRTV()
	{
		return m_renderTargetView.get();
	}
}
