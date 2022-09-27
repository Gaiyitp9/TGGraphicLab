///****************************************************************
//* TianGong RenderLab											*
//* Copyright (c) Gaiyitp9. All rights reserved.					*
//* This code is licensed under the MIT License (MIT).			*
//*****************************************************************/
//
//#include "../Quaternion.hpp"
//
//namespace LCH::Math
//{
//	template<typename T> requires quaternion_type<T>
//	inline Quaternion<T>::Quaternion()
//	{
//		elements[0] = elements[1] = elements[2] = 0;
//		elements[3] = 1;
//	}
//
//	template<typename T> requires quaternion_type<T>
//	inline Quaternion<T>::Quaternion(T x, T y, T z, T w)
//	{
//		elements[0] = x;
//		elements[1] = y;
//		elements[2] = z;
//		elements[3] = w;
//	}
//
//	template<typename T> requires quaternion_type<T>
//	inline Quaternion<T>::Quaternion(T angle, Vector<T, 3> axis)
//	{
//		axis.Normalize();
//		T halfAngle = static_cast<T>(angle * 0.5 * Deg2Rad);
//		Vector<T, 3> sinu = static_cast<T>(sin(halfAngle) * axis);
//		elements[0] = sinu.x();
//		elements[0] = sinu.y();
//		elements[0] = sinu.z();
//		elements[3] = cos(halfAngle);
//	}
//
//	template<typename T> requires quaternion_type<T>
//	inline Quaternion<T>::Quaternion(Vector<T, 3> euler)
//	{
//
//	}
//
//	template<typename T> requires quaternion_type<T>
//	inline const T& Quaternion<T>::x() const
//	{
//		return elements[0];
//	}
//
//	template<typename T> requires quaternion_type<T>
//	inline const T& Quaternion<T>::y() const
//	{
//		return elements[1];
//	}
//
//	template<typename T> requires quaternion_type<T>
//	inline const T& Quaternion<T>::z() const
//	{
//		return elements[2];
//	}
//
//	template<typename T> requires quaternion_type<T>
//	inline const T& Quaternion<T>::w() const
//	{
//		return elements[3];
//	}
//
//	template<typename T> requires quaternion_type<T>
//	inline T& Quaternion<T>::x()
//	{
//		return elements[0];
//	}
//
//	template<typename T> requires quaternion_type<T>
//	inline T& Quaternion<T>::y()
//	{
//		return elements[1];
//	}
//
//	template<typename T> requires quaternion_type<T>
//	inline T& Quaternion<T>::z()
//	{
//		return elements[2];
//	}
//
//	template<typename T> requires quaternion_type<T>
//	inline T& Quaternion<T>::w()
//	{
//		return elements[3];
//	}
//
//	template<typename T> requires quaternion_type<T>
//	inline Quaternion<T> Quaternion<T>::normalized() const
//	{
//		T norm = static_cast<T>(sqrt(Dot(*this)));
//		if (Abs(norm) < epsilon)
//			return {};
//
//		Quaternion normalized;
//		if constexpr (support_simd_t)
//		{
//			Vector<T, 4> normVec(norm);
//			simd::elementwise_div(elements.data(), normVec.data(), normalized.elements.data());
//		}
//		else
//		{
//			for (size_t i = 0; i < 4; ++i)
//				normalized.elements[i] = elements[i] / norm;
//		}
//		return normalized;
//	}
//
//	template<typename T> requires quaternion_type<T>
//	inline T Quaternion<T>::Dot(Quaternion q)
//	{
//		T dot;
//		if constexpr (support_simd_t)
//			dot = simd::dot(elements.data(), q.elements.data());
//		else
//		{
//			dot = 0;
//			for (size_t i = 0; i < 4; ++i)
//				dot += elements[i] * q.elements[i];
//		}
//		return dot;
//	}
//}
