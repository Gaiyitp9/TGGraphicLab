/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Base/Interfaces.h"

namespace TG::Rendering
{
	class IContext
	{
	public:
		virtual ~IContext() = default;

		[[nodiscard]] virtual const IVideoPort& VideoPort() const = 0;
	};
}
