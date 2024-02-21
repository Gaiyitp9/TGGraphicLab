/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Graphics
{
	struct IDeviceContext
	{
		virtual void SetPipelineState() = 0;
		virtual void SetVertexBuffers() = 0;
		virtual void SetIndexBuffer() = 0;
		virtual void SetViewports() = 0;
		virtual void SetRenderTargets() = 0;
		virtual void Draw() = 0;
		virtual void DrawIndexed() = 0;
	};
}
