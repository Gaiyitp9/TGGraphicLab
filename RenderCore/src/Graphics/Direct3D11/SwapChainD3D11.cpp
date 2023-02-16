/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Graphics/Direct3D11/SwapChainD3D11.hpp"

namespace TG::Graphics
{
	SwapChainD3D11::SwapChainD3D11(const winrt::com_ptr<IDXGISwapChain>& swapChain)
	{
		m_d3dSwapChain = swapChain;
	}
}
