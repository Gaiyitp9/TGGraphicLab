/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "../Diagnostics/MemoryDbg.hpp"

namespace LCH::Math
{
	// 地址是否满足对齐条件
	template<typename T>
	inline bool IsAligned(T const* value, size_t alignment)
	{
		return 0 == ((size_t)value & (alignment - 1));
	}

	// 绝对值
	template<typename T>
	inline T Abs(T a)
	{
		return a < 0 ? -a : a;
	}
}