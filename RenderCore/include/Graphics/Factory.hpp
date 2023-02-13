/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once
#include "GraphicsBase.hpp"

namespace TG::Graphics
{
	struct IFactory
	{
		virtual bool EnumAdapters(unsigned int index, AdapterDesc& desc) const = 0;
	};
}
