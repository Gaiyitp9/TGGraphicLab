/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Graphics
{
	struct IRenderDevice
	{
		virtual void CreateBuffer() = 0;
		virtual void CreateShader() = 0;
		virtual void CreateTexture() = 0;
	};
}
