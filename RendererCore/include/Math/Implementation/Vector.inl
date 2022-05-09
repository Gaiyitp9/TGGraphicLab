/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "../Vector.hpp"

namespace LCH::Math
{
	template<typename T, size_t Size> requires mathlib_type_and_size<T, Size>
	Vector<T, Size>::Vector(T x)
	{
		elements.fill(x);
	}

	template<typename T, size_t Size> requires mathlib_type_and_size<T, Size>
	const T& Vector<T, Size>::operator[](size_t index) const
	{
		return elements.at(index);
	}

	template<typename T, size_t Size> requires mathlib_type_and_size<T, Size>
	T& Vector<T, Size>::operator[](size_t index)
	{
		return elements.at(index);
	}

	template<typename T, size_t Size> requires mathlib_type_and_size<T, Size>
	Vector<T, Size> Vector<T, Size>::operator+(const Vector& vec) const
	{
		Vector res;
		if constexpr (support_simd_t)
		{
			T const* v1 = elements.data();
			T const* v2 = vec.elements.data();
			T* vr = res.elements.data();
			for (size_t i = 0; i < loop; ++i)
			{
				simd::add(v1, v2, vr);
				v1 += simd::DataCount;
				v2 += simd::DataCount;
				vr += simd::DataCount;
			}
			for (size_t i = 0; i < remainder; ++i)
			{
				vr[i] = v1[i] + v2[i];
			}
		}
		else
		{
			for (size_t i = 0; i < Size; ++i)
			{
				res.elements[i] = elements[i] + vec.elements[i];
			}
		}
		return res;
	}

	template<typename T> requires mathlib_type_and_size<T, 4>
	Vector<T, 4>::Vector(T x)
	{
		elements.fill(x);
	}

	template<typename T> requires mathlib_type_and_size<T, 4>
	Vector<T, 4>::Vector(T x, T y, T z, T w)
	{
		elements[0] = x;
		elements[1] = y;
		elements[2] = z;
		elements[3] = w;
	}

	template<typename T> requires mathlib_type_and_size<T, 4>
	const T& Vector<T, 4>::operator[](size_t pos) const
	{
		return elements.at(pos);
	}

	template<typename T> requires mathlib_type_and_size<T, 4>
	T& Vector<T, 4>::operator[](size_t pos)
	{
		return elements.at(pos);
	}

	template<typename T> requires mathlib_type_and_size<T, 4>
	const T& Vector<T, 4>::x() const { return elements[0]; }

	template<typename T> requires mathlib_type_and_size<T, 4>
	const T& Vector<T, 4>::y() const { return elements[1]; }

	template<typename T> requires mathlib_type_and_size<T, 4>
	const T& Vector<T, 4>::z() const { return elements[2]; }

	template<typename T> requires mathlib_type_and_size<T, 4>
	const T& Vector<T, 4>::w() const { return elements[3]; }

	template<typename T> requires mathlib_type_and_size<T, 4>
	T& Vector<T, 4>::x() { return elements[0]; }

	template<typename T> requires mathlib_type_and_size<T, 4>
	T& Vector<T, 4>::y() { return elements[1]; }

	template<typename T> requires mathlib_type_and_size<T, 4>
	T& Vector<T, 4>::z() { return elements[2]; }

	template<typename T> requires mathlib_type_and_size<T, 4>
	T& Vector<T, 4>::w() { return elements[3]; }

	template<typename T> requires mathlib_type_and_size<T, 4>
	T Vector<T, 4>::magnitude() const
	{
		return sqrt(Dot(*this));
	}

	template<typename T> requires mathlib_type_and_size<T, 4>
	T Vector<T, 4>::sqrMagnitude() const
	{
		return Dot(*this);
	}

	template<typename T> requires mathlib_type_and_size<T, 4>
	Vector<T, 4> Vector<T, 4>::normalized() const
	{
		return *this / magnitude();
	}

	template<typename T> requires mathlib_type_and_size<T, 4>
	T Vector<T, 4>::Dot(const Vector& vec) const
	{
		T dot;
		if constexpr (support_simd_t)
			dot = simd::dot(elements.data(), vec.elements.data());
		else
		{
			dot = 0;
			for (size_t i = 0; i < 4; ++i)
				dot += elements[i] * vec.elements[i];
		}
		return dot;
	}

	template<typename T> requires mathlib_type_and_size<T, 4>
	Vector<T, 4> Vector<T, 4>::operator+(const Vector& vec) const
	{
		Vector res;
		if constexpr (support_simd_t)
			simd::add(elements.data(), vec.elements.data(), res.elements.data());
		else
		{
			for (size_t i = 0; i < 4; ++i)
			{
				res.elements[i] = elements[i] + vec.elements[i];
			}
		}
		return res;
	}

	template<typename T> requires mathlib_type_and_size<T, 4>
	Vector<T, 4> Vector<T, 4>::operator-(const Vector& vec) const
	{
		Vector res;
		if constexpr (support_simd_t)
			simd::sub(elements.data(), vec.elements.data(), res.elements.data());
		else
		{
			for (size_t i = 0; i < 4; ++i)
			{
				res.elements[i] = elements[i] - vec.elements[i];
			}
		}
		return res;
	}

	template<typename T> requires mathlib_type_and_size<T, 4>
	Vector<T, 4> Vector<T, 4>::operator*(T a) const
	{
		Vector res(a);
		simd::elementwise_product(elements.data(), res.elements.data(), res.elements.data());
		return res;
	}

	template<typename T> requires mathlib_type_and_size<T, 4>
	Vector<T, 4> Vector<T, 4>::operator/(T a) const
	{
		Vector res(a);
		simd::elementwise_div(elements.data(), res.elements.data(), res.elements.data());
		return res;
	}

	template<typename T, typename U, size_t Size> 
		requires mathlib_type_and_size<T, Size> && std::is_arithmetic_v<U>
	Vector<T, Size> operator*(U a, Vector<T, Size> v)
	{
		return v * static_cast<T>(a);
	}

	template<typename T> requires mathlib_type_and_size<T, 3>
	Vector<T, 3>::Vector(T x)
	{
		elements.fill(x);
	}

	template<typename T> requires mathlib_type_and_size<T, 3>
	Vector<T, 3>::Vector(T x, T y, T z)
	{
		elements[0] = x;
		elements[1] = y;
		elements[2] = z;
	}

	template<typename T> requires mathlib_type_and_size<T, 3>
	const T& Vector<T, 3>::operator[](size_t index) const
	{
		return elements.at(index);
	}

	template<typename T> requires mathlib_type_and_size<T, 3>
	T& Vector<T, 3>::operator[](size_t index)
	{
		return elements.at(index);
	}

	template<typename T> requires mathlib_type_and_size<T, 3>
	const T& Vector<T, 3>::x() const { return elements[0]; }

	template<typename T> requires mathlib_type_and_size<T, 3>
	const T& Vector<T, 3>::y() const { return elements[1]; }

	template<typename T> requires mathlib_type_and_size<T, 3>
	const T& Vector<T, 3>::z() const { return elements[2]; }

	template<typename T> requires mathlib_type_and_size<T, 3>
	T& Vector<T, 3>::x() { return elements[0]; }

	template<typename T> requires mathlib_type_and_size<T, 3>
	T& Vector<T, 3>::y() { return elements[1]; }

	template<typename T> requires mathlib_type_and_size<T, 3>
	T& Vector<T, 3>::z() { return elements[2]; }

	template<typename T> requires mathlib_type_and_size<T, 3>
	T Vector<T, 3>::magnitude() const
	{
		return sqrt(Dot(*this));
	}

	template<typename T> requires mathlib_type_and_size<T, 3>
	T Vector<T, 3>::sqrMagnitude() const
	{
		return Dot(*this);
	}

	template<typename T> requires mathlib_type_and_size<T, 3>
	Vector<T, 3> Vector<T, 3>::normalized() const
	{
		return *this / magnitude();
	}

	template<typename T> requires mathlib_type_and_size<T, 3>
	T Vector<T, 3>::Dot(const Vector& vec) const
	{
		return elements[0] * vec.elements[0] + elements[1] * vec.elements[1] + elements[2] * vec.elements[2];
	}

	template<typename T> requires mathlib_type_and_size<T, 3>
	Vector<T, 3> Vector<T, 3>::operator+(const Vector& vec) const
	{
		Vector res;
		for (size_t i = 0; i < 3; ++i)
			res.elements[i] = elements[i] + vec.elements[i];
		return res;
	}

	template<typename T> requires mathlib_type_and_size<T, 3>
	Vector<T, 3> Vector<T, 3>::operator-(const Vector& vec) const
	{
		Vector res;
		for (size_t i = 0; i < 3; ++i)
			res.elements[i] = elements[i] - vec.elements[i];
		return res;
	}

	template<typename T> requires mathlib_type_and_size<T, 3>
	Vector<T, 3> Vector<T, 3>::operator*(T a) const
	{
		Vector res;
		for (size_t i = 0; i < 3; ++i)
			res.elements[i] = elements[i] * a;
		return res;
	}

	template<typename T> requires mathlib_type_and_size<T, 3>
	Vector<T, 3> Vector<T, 3>::operator/(T a) const
	{
		Vector res;
		for (size_t i = 0; i < 3; ++i)
			res.elements[i] = elements[i] / a;
		return res;
	}

	template<typename T> requires mathlib_type_and_size<T, 2>
	Vector<T, 2>::Vector(T x)
	{
		elements.fill(x);
	}

	template<typename T> requires mathlib_type_and_size<T, 2>
	Vector<T, 2>::Vector(T x, T y)
	{
		elements[0] = x;
		elements[1] = y;
	}

	template<typename T> requires mathlib_type_and_size<T, 2>
	const T& Vector<T, 2>::operator[](size_t pos) const
	{
		return elements.at(pos);
	}

	template<typename T> requires mathlib_type_and_size<T, 2>
	T& Vector<T, 2>::operator[](size_t pos)
	{
		return elements.at(pos);
	}

	template<typename T> requires mathlib_type_and_size<T, 2>
	const T& Vector<T, 2>::x() const { return elements[0]; }

	template<typename T> requires mathlib_type_and_size<T, 2>
	const T& Vector<T, 2>::y() const { return elements[1]; }

	template<typename T> requires mathlib_type_and_size<T, 2>
	T& Vector<T, 2>::x() { return elements[0]; }

	template<typename T> requires mathlib_type_and_size<T, 2>
	T& Vector<T, 2>::y() { return elements[1]; }

	template<typename T> requires mathlib_type_and_size<T, 2>
	T Vector<T, 2>::magnitude() const
	{
		return sqrt(Dot(*this));
	}

	template<typename T> requires mathlib_type_and_size<T, 2>
	T Vector<T, 2>::sqrMagnitude() const
	{
		return Dot(*this);
	}

	template<typename T> requires mathlib_type_and_size<T, 2>
	Vector<T, 2> Vector<T, 2>::normalized() const
	{
		return *this / magnitude();
	}

	template<typename T> requires mathlib_type_and_size<T, 2>
	T Vector<T, 2>::Dot(const Vector& vec) const
	{
		return elements[0] * vec.elements[0] + elements[1] * vec.elements[1];
	}

	template<typename T> requires mathlib_type_and_size<T, 2>
	Vector<T, 2> Vector<T, 2>::operator+(const Vector& vec) const
	{
		Vector res;
		for (size_t i = 0; i < 2; ++i)
			res.elements[i] = elements[i] + vec.elements[i];
		return res;
	}

	template<typename T> requires mathlib_type_and_size<T, 2>
	Vector<T, 2> Vector<T, 2>::operator-(const Vector& vec) const
	{
		Vector res;
		res.elements[0] = elements[0] - vec.elements[0];
		res.elements[1] = elements[1] - vec.elements[1];
		return res;
	}

	template<typename T> requires mathlib_type_and_size<T, 2>
	Vector<T, 2> Vector<T, 2>::operator*(T a) const
	{
		Vector res;
		res.elements[0] = elements[0] * a;
		res.elements[1] = elements[1] * a;
		return res;
	}

	template<typename T> requires mathlib_type_and_size<T, 2>
	Vector<T, 2> Vector<T, 2>::operator/(T a) const
	{
		Vector res;
		res.elements[0] = elements[0] / a;
		res.elements[1] = elements[1] / a;
		return res;
	}
}