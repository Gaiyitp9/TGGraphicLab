/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Graphics/DxgiDbgInfo.hpp"

namespace LCH::Graphics
{
	DxgiDbgInfo::DxgiDbgInfo()
	{
		HMODULE module = GetModuleHandleW(L"dxgidebug.dll");
		DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
			reinterpret_cast<void*>(GetProcAddress(module, "DXGIGetDebugInterface")));
		DxgiGetDebugInterface(IID_PPV_ARGS(&dxgiInfoQueue));
	}
}