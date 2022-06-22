/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Simd.hpp"

namespace LCH::Math
{
	template <typename T> requires std::is_same_v<T, double> || std::is_same_v<T, float>
	class Quaternion
	{
		using simd = simd_trait<T, SimdInstruction<T, 4>::type>;
	public:
		Quaternion();
		Quaternion(T x, T y, T z, T w);

	private:
		aligned_array<T, 4, simd::Alignment> elements;
	};

	using Quaternionf = Quaternion<float>;
	using Quaterniond = Quaternion<double>;
}