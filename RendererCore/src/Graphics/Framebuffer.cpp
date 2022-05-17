/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Graphics/Framebuffer.h"

namespace LCH::Graphics
{
	void Framebuffer::ClearRenderTarget(float red, float green, float blue)
	{
		const float color[] = { red, green, blue, 1.0f };
		//pContext->ClearRenderTargetView(pRenderTargetView.Get(), color);
	}
}