/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Diagnostics/MemoryDbg.h"

namespace LCH::Math
{
	// 数学工具类
	class MathUtil
	{
	public:
		// 地址是否满足对齐条件
		template <typename T>
		inline static bool IsAligned(T const* value, size_t alignment)
		{
			return 0 == ((size_t)value & (alignment - 1));
		}
	};
}