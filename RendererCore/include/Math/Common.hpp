/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "../Diagnostics/MemoryDbg.hpp"

namespace TG::Math
{

inline constexpr double PI = 3.14159265358979;
inline constexpr double Rad2Deg = 57.2957795130823;
inline constexpr double Deg2Rad = 0.0174532925199433;

// 地址是否满足对齐条件
template<typename T>
inline bool IsAligned(T const* value, size_t alignment)
{
	return 0 == ((size_t)value & (alignment - 1));
}

template<typename T> inline T Abs(T a) { return a < 0 ? -a : a; }

}