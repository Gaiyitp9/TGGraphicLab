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
		SwapChainD3D11();

		virtual void Present(int syncInterval);
		virtual void Resize(int width, int height);

	private:
		winrt::com_ptr<IDXGISwapChain> m_d3dSwapChain;
	};
}
