/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Window/Window.hpp"
#include "../Math/Color.hpp"

namespace TG::Graphics
{
	enum class LowLevelAPI : unsigned char
	{
		DirectX11,
		DirectX12,
		OpenGL,
		Vulcan,
	};

	// 图形设备
	template <LowLevelAPI API>
	class Device;

	// 设备上下文
	template <LowLevelAPI API>
	class Context;

	// 帧缓存
	template <LowLevelAPI API>
	class FrameBuffer;

	// 缓存格式
	enum class Format
	{
		R8G8B8A8_UNORM = 28,
	};

#ifdef _DEBUG
	template<LowLevelAPI API>
	class DebugInfo;
#endif
}