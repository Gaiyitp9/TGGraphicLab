/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Vector.hpp"

namespace LCH::Math
{
	template<typename T>
	concept quaternion_type = std::is_same_v<T, double> || std::is_same_v<T, float>;

	template<typename T> requires quaternion_type<T>
	class Quaternion
	{
		using simd = simd_trait<T, typename SimdInstruction<T, 4>::type>;
		static constexpr T epsilon = static_cast<T>(1e-6);
	public:
		Quaternion();
		Quaternion(T x, T y, T z, T w);
		Quaternion(T angle, Vector<T, 3> axis);
		Quaternion(Vector<T, 3> euler);				// 欧拉角环绕顺序为heading-pitch-bank(yaw-pitch-roll)
													// Y-X-Z(Object Space)或Z-X-Y(World Space 或 Parent Space)
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
		T Dot(Quaternion q);

	private:
		aligned_array<T, 4, simd::Alignment> elements;
	};

	using Quaternionf = Quaternion<float>;
	using Quaterniond = Quaternion<double>;
}

#include "Implementation/Quaternion.inl"