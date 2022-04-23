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
	template<typename T, size_t Size> requires mathlib_type_and_size<T, Size>
	class Vector
	{
		using simd = simd_trait<T>;
	public:
		Vector()
		{

		}

		Vector operator+(const Vector& v)
		{
			if constexpr (support_simd_t)
			{

			}
			else
			{

			}
		}

	private:
		aligned_array<T, Size> data;
	};

	template<typename T> requires mathlib_type_and_size<T, 4>
	class Vector<T, 4>
	{
		using simd = simd_trait<T>;

	public:
		Vector();
		T x() { return data[0]; }
		T y() { return data[1]; }
		T z() { return data[2]; }
		T w() { return data[3]; }

	private:
		aligned_array<T, 4, simd::Alignment> data;
	};
}