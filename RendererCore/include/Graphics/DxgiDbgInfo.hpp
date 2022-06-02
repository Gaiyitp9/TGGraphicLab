/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <dxgidebug.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

namespace LCH::Graphics
{
	class DxgiDbgInfo
	{
	public:
		DxgiDbgInfo();

	private:
		using DXGIGetDebugInterface = HRESULT(*)(REFIID, void**);
		DXGIGetDebugInterface DxgiGetDebugInterface;

		ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
	};
}