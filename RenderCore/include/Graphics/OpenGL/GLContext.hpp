/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once
#include "PlatformHeaders.h"
#include "glad/gl.h"
#include "GLCreateInfo.hpp"

namespace TG::Graphics
{
	class GLContext
	{
	public:
		GLContext(const GLCreateInfo& info);
	};
}
