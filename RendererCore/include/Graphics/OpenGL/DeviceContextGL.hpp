/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Graphics
{
	class DeviceContextGL final : public IDeviceContext
	{
	
	private:
		GLContext m_context;
	};
}
