/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Vector.hpp"

namespace LCH::Math
{
	template<typename T> requires std::is_same_v<T, double> || std::is_same_v<T, float>
	class Quaternion
	{
		using simd = simd_trait<T, typename SimdInstruction<T, 4>::type>;
	public:
		Quaternion();
		Quaternion(T x, T y, T z, T w);

		const T& x() const;
		const T& y() const;
		const T& z() const;
		const T& w() const;
		T& x();
		T& y();
		T& z();
		T& w();

		Quaternion normalized() const;

	public:
		static Quaternion AngleAxis(T anlge, Vector<T, 3> axis);

	public:
		T Dot(Quaternion q);

	private:
		aligned_array<T, 4, simd::Alignment> elements;
	};

	using Quaternionf = Quaternion<float>;
	using Quaterniond = Quaternion<double>;
}