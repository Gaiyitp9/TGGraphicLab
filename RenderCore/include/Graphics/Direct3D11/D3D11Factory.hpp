/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once
#include "../Factory.hpp"
#include <winrt/base.h>
#include <dxgi.h>

namespace TG::Graphics
{
	class D3D11Factory final : public IFactory
	{
	public:
		D3D11Factory();

		virtual bool EnumAdapters(unsigned int index, AdapterDesc& desc) const override;

	private:
		winrt::com_ptr<IDXGIFactory1> m_dxgiFactory;
	};
}
