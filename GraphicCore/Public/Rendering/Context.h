/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Base/CommonInterfaces.h"

namespace TG::Rendering
{
	class Context
	{
	public:
		virtual ~Context() = default;

		virtual const IDefaultVideoPort& VideoPort() const = 0;
	};
}
