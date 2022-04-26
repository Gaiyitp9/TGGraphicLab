/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Simd.hpp"

namespace LCH::Math
{
	template<typename T, size_t Size> requires mathlib_type_and_size<T, Size>
	class Vector
	{
		using simd = simd_trait<T, SimdInstruction<Size>::type>;
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
		aligned_array<T, Size, simd::Alignment> data;
	};

	template<typename T> requires mathlib_type_and_size<T, 4>
	class Vector<T, 4>
	{
		using simd = simd_trait<T, SimdInstruction<4>::type>;

	public:
		Vector(T x = {})
		{
			data.fill(x);
		}

		Vector(T x, T y, T z, T w)
		{
			data[0] = x;
			data[1] = y;
			data[2] = z;
			data[3] = w;
		}

		const T& operator[](size_t pos) const
		{
			return data.at(pos);
		}

		T& operator[](size_t pos)
		{
			return data.at(pos);
		}

		const T& x() const { return data[0]; }
		const T& y() const { return data[1]; }
		const T& z() const { return data[2]; }
		const T& w() const { return data[3]; }
		T& x() { return data[0]; }
		T& y() { return data[1]; }
		T& z() { return data[2]; }
		T& w() { return data[3]; }

	private:
		aligned_array<T, 4> data;
	};
}