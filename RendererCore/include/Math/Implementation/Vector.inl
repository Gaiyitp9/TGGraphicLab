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
		data.fill(x);
	}

	template<typename T, size_t Size> requires mathlib_type_and_size<T, Size>
	const T& Vector<T, Size>::operator[](size_t index) const
	{
		return data.at(index);
	}

	template<typename T, size_t Size> requires mathlib_type_and_size<T, Size>
	T& Vector<T, Size>::operator[](size_t index)
	{
		return data.at(index);
	}

	template<typename T, size_t Size> requires mathlib_type_and_size<T, Size>
	Vector<T, Size> Vector<T, Size>::operator+(const Vector& vec) const
	{
		Vector res;
		if constexpr (support_simd_t)
		{
			T const* v1 = data.data();
			T const* v2 = vec.data.data();
			T* vr = res.data.data();
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
				res.data[i] = data[i] + vec.data[i];
			}
		}
		return res;
	}

	template<typename T> requires mathlib_type_and_size<T, 4>
	Vector<T, 4>::Vector(T x)
	{
		data.fill(x);
	}

	template<typename T> requires mathlib_type_and_size<T, 4>
	Vector<T, 4>::Vector(T x, T y, T z, T w)
	{
		data[0] = x;
		data[1] = y;
		data[2] = z;
		data[3] = w;
	}

	template<typename T> requires mathlib_type_and_size<T, 4>
	const T& Vector<T, 4>::operator[](size_t pos) const
	{
		return data.at(pos);
	}

	template<typename T> requires mathlib_type_and_size<T, 4>
	T& Vector<T, 4>::operator[](size_t pos)
	{
		return data.at(pos);
	}

	template<typename T> requires mathlib_type_and_size<T, 4>
	const T& Vector<T, 4>::x() const { return data[0]; }

	template<typename T> requires mathlib_type_and_size<T, 4>
	const T& Vector<T, 4>::y() const { return data[1]; }

	template<typename T> requires mathlib_type_and_size<T, 4>
	const T& Vector<T, 4>::z() const { return data[2]; }

	template<typename T> requires mathlib_type_and_size<T, 4>
	const T& Vector<T, 4>::w() const { return data[3]; }

	template<typename T> requires mathlib_type_and_size<T, 4>
	T& Vector<T, 4>::x() { return data[0]; }

	template<typename T> requires mathlib_type_and_size<T, 4>
	T& Vector<T, 4>::y() { return data[1]; }

	template<typename T> requires mathlib_type_and_size<T, 4>
	T& Vector<T, 4>::z() { return data[2]; }

	template<typename T> requires mathlib_type_and_size<T, 4>
	T& Vector<T, 4>::w() { return data[3]; }

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
			dot = simd::dot(data.data(), vec.data.data());
		else
			dot = data[0] * vec.data[0] + data[1] * vec.data[1] + data[2] * vec.data[2] + data[3] * vec.data[3];
		return dot;
	}

	template<typename T> requires mathlib_type_and_size<T, 4>
	Vector<T, 4> Vector<T, 4>::operator+(const Vector& vec) const
	{
		Vector res;
		if constexpr (support_simd_t)
			simd::add(data.data(), vec.data.data(), res.data.data());
		else
		{
			for (size_t i = 0; i < 4; ++i)
			{
				res.data[i] = data[i] + vec.data[i];
			}
		}
		return res;
	}

	template<typename T> requires mathlib_type_and_size<T, 4>
	Vector<T, 4> Vector<T, 4>::operator-(const Vector& vec) const
	{
		Vector res;
		if constexpr (support_simd_t)
			simd::sub(data.data(), vec.data.data(), res.data.data());
		else
		{
			for (size_t i = 0; i < 4; ++i)
			{
				res.data[i] = data[i] - vec.data[i];
			}
		}
		return res;
	}

	template<typename T> requires mathlib_type_and_size<T, 4>
	Vector<T, 4> Vector<T, 4>::operator*(T a) const
	{
		Vector res(a);
		simd::elementwise_product(data.data(), res.data.data(), res.data.data());
		return res;
	}

	template<typename T> requires mathlib_type_and_size<T, 4>
	Vector<T, 4> Vector<T, 4>::operator/(T a) const
	{
		Vector res(a);
		simd::elementwise_div(data.data(), res.data.data(), res.data.data());
		return res;
	}

	template<typename T, typename U, size_t Size> 
		requires mathlib_type_and_size<T, Size> && std::is_arithmetic_v<U>
	Vector<T, Size> operator*(U a, Vector<T, Size> v)
	{
		return v * static_cast<T>(a);
	}

	template<typename T> requires mathlib_type_and_size<T, 2>
	Vector<T, 2>::Vector(T x)
	{
		data.fill(x);
	}

	template<typename T> requires mathlib_type_and_size<T, 2>
	Vector<T, 2>::Vector(T x, T y)
	{
		data[0] = x;
		data[1] = y;
	}

	template<typename T> requires mathlib_type_and_size<T, 2>
	const T& Vector<T, 2>::operator[](size_t pos) const
	{
		return data.at(pos);
	}

	template<typename T> requires mathlib_type_and_size<T, 2>
	T& Vector<T, 2>::operator[](size_t pos)
	{
		return data.at(pos);
	}

	template<typename T> requires mathlib_type_and_size<T, 2>
	const T& Vector<T, 2>::x() const { return data[0]; }

	template<typename T> requires mathlib_type_and_size<T, 2>
	const T& Vector<T, 2>::y() const { return data[1]; }

	template<typename T> requires mathlib_type_and_size<T, 2>
	T& Vector<T, 2>::x() { return data[0]; }

	template<typename T> requires mathlib_type_and_size<T, 2>
	T& Vector<T, 2>::y() { return data[1]; }

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
		return data[0] * vec.data[0] + data[1] * vec.data[1];
	}

	template<typename T> requires mathlib_type_and_size<T, 2>
	Vector<T, 2> Vector<T, 2>::operator+(const Vector& vec) const
	{
		Vector res;
		res.data[0] = data[0] + vec.data[0];
		res.data[1] = data[1] + vec.data[1];
		return res;
	}

	template<typename T> requires mathlib_type_and_size<T, 2>
	Vector<T, 2> Vector<T, 2>::operator-(const Vector& vec) const
	{
		Vector res;
		res.data[0] = data[0] - vec.data[0];
		res.data[1] = data[1] - vec.data[1];
		return res;
	}

	template<typename T> requires mathlib_type_and_size<T, 2>
	Vector<T, 2> Vector<T, 2>::operator*(T a) const
	{
		Vector res;
		res.data[0] = data[0] * a;
		res.data[1] = data[1] * a;
		return res;
	}

	template<typename T> requires mathlib_type_and_size<T, 2>
	Vector<T, 2> Vector<T, 2>::operator/(T a) const
	{
		Vector res;
		res.data[0] = data[0] / a;
		res.data[1] = data[1] / a;
		return res;
	}
}