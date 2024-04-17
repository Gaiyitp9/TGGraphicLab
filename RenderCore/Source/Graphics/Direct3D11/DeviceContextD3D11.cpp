/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Graphics/Direct3D11/DeviceContextD3D11.hpp"

namespace TG::Graphics
{
	DeviceContextD3D11::DeviceContextD3D11(const CreateInfoD3D11& info, const winrt::com_ptr<ID3D11DeviceContext>& context)
	{
		m_context = context;
	}
}
