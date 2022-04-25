/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Simd.hpp"
#include <stdexcept>

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

		T operator[](size_t index)
		{
			return data.at(index);
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
		Vector()
		{
			value = _mm_setzero_ps();
		}

		Vector(float x)
		{
			value = _mm_set_ps1(x);
		}

		Vector(float x, float y, float z, float w)
		{
			value = _mm_set_ps(x, y, z, w);
		}

		T operator[](size_t pos) const
		{
			if (pos > 3)
				throw std::out_of_range();

			return value.m128_f32[pos];
		}

		T x() { return value.m128_f32[0]; }
		T y() { return value.m128_f32[1]; }
		T z() { return value.m128_f32[2]; }
		T w() { return value.m128_f32[3]; }

	private:
		__m128 value;
	};
}