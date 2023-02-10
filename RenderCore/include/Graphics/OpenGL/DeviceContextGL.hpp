/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once
#include "glad/gl.h"
#include "../DeviceContext.hpp"
#include "GLContext.hpp"

namespace TG::Graphics
{
	class DeviceContextGL final : public IDeviceContext
	{
	public:
		DeviceContextGL(const GLCreateInfo& info);

	private:
		GLContext m_context;
	};
}
