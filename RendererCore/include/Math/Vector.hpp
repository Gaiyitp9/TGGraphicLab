/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Simd.hpp"
#include "MathUtil.h"

namespace LCH::Math
{
	template<typename T, size_t N>
	class Vector
	{
		using simd = simd_trait<T>;
		
	public:
		Vector();

		Vector operator+(const Vector& lhs, const Vector& rhs)
		{
			if constexpr (support_simd::value)
			{

			}
			else
			{

			}
		}

	private:
		aligned_array<T, N, simd::Alignment> data;
	};

	template<typename T>
	class Vector<T, 4>
	{
		using simd = simd_trait<T>;

	public:
		Vector();

	private:
		aligned_array<T, 4, simd::Alignment> data;
	};
}