/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once
#include <dxgiformat.h>
#include "../GraphicsBase.hpp"

namespace TG::Graphics
{
	DXGI_FORMAT TexFormatToDXGIFormat(TextureFormat fmt)
	{
		switch (fmt)
		{
		case TextureFormat::UNKNOWN:		return DXGI_FORMAT_UNKNOWN;
		case TextureFormat::R8G8B8A8_UNORM:	return DXGI_FORMAT_R8G8B8A8_UNORM;
		default:							return DXGI_FORMAT_UNKNOWN;
		}
	}
}
