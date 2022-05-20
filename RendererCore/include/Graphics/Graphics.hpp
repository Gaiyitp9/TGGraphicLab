/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Diagnostics/MemoryDbg.h"		// 必须放在文件头部，保证malloc使用的是debug版本
#include "../Window.h"

namespace LCH::Graphics
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

	// 有理数，用来记录刷新率
	struct Rational
	{
		unsigned int numerator;
		unsigned int denominator;
	};

	// 缓存格式
	enum class Format
	{
		R8G8B8A8_UNORM = 28,
	};
}