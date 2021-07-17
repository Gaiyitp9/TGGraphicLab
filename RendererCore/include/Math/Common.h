/****************************************************************
* Copyright (c) IGOTGAMES(IGG ShenZhen). All rights reserved.	*
*																*
* This code is licensed under the MIT License (MIT).			*
* THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF				*
* ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY			*
* IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR				*
* PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.				*
*																*
* Author: LiaoChenhan											*
*****************************************************************/

#pragma once

#include <DirectXMath.h>
#include <intrin.h>

#define INLINE __forceinline

namespace Math
{
	template <typename T> __forceinline bool IsAligned(T value, size_t alignment)
	{
		return 0 == ((size_t)value & (alignment - 1));
	}
}