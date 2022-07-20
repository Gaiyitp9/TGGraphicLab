/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "../Quaternion.hpp"

namespace LCH::Math
{
	template<typename T> requires quaternion_type<T>
	inline Quaternion<T>::Quaternion()
	{
		elements[0] = elements[1] = elements[2] = 0;
		elements[3] = 1;
	}

	template<typename T> requires quaternion_type<T>
	inline Quaternion<T>::Quaternion(T x, T y, T z, T w)
	{
		elements[0] = x;
		elements[1] = y;
		elements[2] = z;
		elements[3] = w;
	}

	template<typename T> requires quaternion_type<T>
	inline Quaternion<T>::Quaternion(T angle, Vector<T, 3> axis)
	{
		axis.Normalize();
		T halfAngle = angle / 2;
		Vector<T, 3> sinu = static_cast<T>(sin(halfAngle)) * axis;
		elements[0] = sinu.x();
		elements[0] = sinu.y();
		elements[0] = sinu.z();
		elements[3] = cos(halfAngle);
	}

	template<typename T> requires quaternion_type<T>
	inline Quaternion<T> Quaternion<T>::normalized() const
	{
		Quaternion normalized;
		if constexpr (support_simd_t)
		{
			Vector<T, 4> norm(sqrt(Dot(*this)));
			simd::elementwise_div(elements.data(), norm.data(), normalized.elements.data());
		}
		else
		{
			T norm = sqrt(Dot(*this));
			for (size_t i = 0; i < 4; ++i)
				normalized.elements[i] = elements[i] / norm;
		}
		return normalized;
	}

	template<typename T> requires quaternion_type<T>
	inline T Quaternion<T>::Dot(Quaternion q)
	{
		T dot;
		if constexpr (support_simd_t)
			dot = simd::dot(elements.data(), q.elements.data());
		else
		{
			dot = 0;
			for (size_t i = 0; i < 4; ++i)
				dot += elements[i] * q.elements[i];
		}
		return dot;
	}
}
