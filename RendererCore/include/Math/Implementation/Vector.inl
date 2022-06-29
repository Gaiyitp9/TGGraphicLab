/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "../Vector.hpp"

namespace LCH::Math
{
	template<typename T, size_t Dimension> requires type_and_dimension<T, Dimension>
	inline Vector<T, Dimension>::Vector(T x)
	{
		elements.fill(x);
	}

	template<typename T, size_t Dimension> requires type_and_dimension<T, Dimension>
	inline const T& Vector<T, Dimension>::operator[](size_t index) const
	{
		return elements.at(index);
	}

	template<typename T, size_t Dimension> requires type_and_dimension<T, Dimension>
	inline T& Vector<T, Dimension>::operator[](size_t index)
	{
		return elements.at(index);
	}

	template<typename T, size_t Dimension> requires type_and_dimension<T, Dimension>
	Vector<T, Dimension> Vector<T, Dimension>::operator+(const Vector& vec) const
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
			for (size_t i = 0; i < Dimension; ++i)
			{
				res.elements[i] = elements[i] + vec.elements[i];
			}
		}
		return res;
	}

	template<typename T>
	inline Vector<T, 4>::Vector(T x)
	{
		elements.fill(x);
	}

	template<typename T>
	inline Vector<T, 4>::Vector(T x, T y, T z, T w)
	{
		elements[0] = x;
		elements[1] = y;
		elements[2] = z;
		elements[3] = w;
	}

	template<typename T>
	inline const T& Vector<T, 4>::operator[](size_t pos) const
	{
		return elements.at(pos);
	}

	template<typename T>
	inline T& Vector<T, 4>::operator[](size_t pos)
	{
		return elements.at(pos);
	}

	template<typename T>
	inline const T& Vector<T, 4>::x() const { return elements[0]; }

	template<typename T>
	inline const T& Vector<T, 4>::y() const { return elements[1]; }

	template<typename T>
	inline const T& Vector<T, 4>::z() const { return elements[2]; }

	template<typename T>
	inline const T& Vector<T, 4>::w() const { return elements[3]; }

	template<typename T>
	inline T& Vector<T, 4>::x() { return elements[0]; }

	template<typename T>
	inline T& Vector<T, 4>::y() { return elements[1]; }

	template<typename T>
	inline T& Vector<T, 4>::z() { return elements[2]; }

	template<typename T>
	inline T& Vector<T, 4>::w() { return elements[3]; }

	template<typename T>
	inline T Vector<T, 4>::magnitude() const
	{
		return sqrt(Dot(*this));
	}

	template<typename T>
	inline T Vector<T, 4>::sqrMagnitude() const
	{
		return Dot(*this);
	}

	template<typename T>
	inline Vector<T, 4> Vector<T, 4>::normalized() const
	{
		T mag = magnitude();
		if (mag < epsilon)
			return { 0 };

		return *this / mag;
	}

	template<typename T>
	inline T Vector<T, 4>::Dot(const Vector& vec) const
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

	template<typename T>
	inline Vector<T, 4> Vector<T, 4>::operator+(const Vector& vec) const
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

	template<typename T>
	inline Vector<T, 4> Vector<T, 4>::operator-(const Vector& vec) const
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

	template<typename T>
	inline Vector<T, 4> Vector<T, 4>::operator*(T a) const
	{
		Vector res(a);
		simd::elementwise_product(elements.data(), res.elements.data(), res.elements.data());
		return res;
	}

	template<typename T>
	inline Vector<T, 4> Vector<T, 4>::operator/(T a) const
	{
		if (Utility::Abs(a) < epsilon)
			return { 0 };

		Vector res(a);
		simd::elementwise_div(elements.data(), res.elements.data(), res.elements.data());
		return res;
	}

	template<typename T, typename U, size_t Dimension> 
		requires type_and_dimension<T, Dimension> && std::is_arithmetic_v<U>
	inline Vector<T, Dimension> operator*(U a, Vector<T, Dimension> v)
	{
		return v * static_cast<T>(a);
	}

	template<typename T>
	inline Vector<T, 3>::Vector(T x)
	{
		elements.fill(x);
	}

	template<typename T>
	inline Vector<T, 3>::Vector(T x, T y, T z)
	{
		elements[0] = x;
		elements[1] = y;
		elements[2] = z;
	}

	template<typename T>
	inline const T& Vector<T, 3>::operator[](size_t index) const
	{
		return elements.at(index);
	}

	template<typename T>
	inline T& Vector<T, 3>::operator[](size_t index)
	{
		return elements.at(index);
	}

	template<typename T>
	inline const T& Vector<T, 3>::x() const { return elements[0]; }

	template<typename T>
	inline const T& Vector<T, 3>::y() const { return elements[1]; }

	template<typename T>
	inline const T& Vector<T, 3>::z() const { return elements[2]; }

	template<typename T>
	inline T& Vector<T, 3>::x() { return elements[0]; }

	template<typename T>
	inline T& Vector<T, 3>::y() { return elements[1]; }

	template<typename T>
	inline T& Vector<T, 3>::z() { return elements[2]; }

	template<typename T>
	inline T Vector<T, 3>::magnitude() const
	{
		return sqrt(Dot(*this));
	}

	template<typename T>
	inline T Vector<T, 3>::sqrMagnitude() const
	{
		return Dot(*this);
	}

	template<typename T>
	inline Vector<T, 3> Vector<T, 3>::normalized() const
	{
		T mag = magnitude();
		if (mag < epsilon)
			return { 0 };

		return *this / mag;
	}

	template<typename T>
	inline T Vector<T, 3>::Dot(const Vector& vec) const
	{
		return elements[0] * vec.elements[0] + elements[1] * vec.elements[1] + elements[2] * vec.elements[2];
	}

	template<typename T>
	inline Vector<T, 3> Vector<T, 3>::Cross(const Vector& vec) const
	{
		return { elements[1] * vec.elements[2] - vec.elements[1] * elements[2],
				 vec.elements[0] * elements[2] - elements[0] * vec.elements[2],
				 elements[0] * vec.elements[1] - vec.elements[0] * elements[1] };
	}

	template<typename T>
	inline Vector<T, 3> Vector<T, 3>::operator+(const Vector& vec) const
	{
		Vector res;
		for (size_t i = 0; i < 3; ++i)
			res.elements[i] = elements[i] + vec.elements[i];
		return res;
	}

	template<typename T>
	inline Vector<T, 3> Vector<T, 3>::operator-(const Vector& vec) const
	{
		Vector res;
		for (size_t i = 0; i < 3; ++i)
			res.elements[i] = elements[i] - vec.elements[i];
		return res;
	}

	template<typename T>
	inline Vector<T, 3> Vector<T, 3>::operator*(T a) const
	{
		Vector res;
		for (size_t i = 0; i < 3; ++i)
			res.elements[i] = elements[i] * a;
		return res;
	}

	template<typename T>
	inline Vector<T, 3> Vector<T, 3>::operator/(T a) const
	{
		if (Utility::Abs(a) < epsilon)
			return { 0 };

		Vector res;
		for (size_t i = 0; i < 3; ++i)
			res.elements[i] = elements[i] / a;
		return res;
	}

	template<typename T>
	inline Vector<T, 2>::Vector(T x)
	{
		elements.fill(x);
	}

	template<typename T>
	inline Vector<T, 2>::Vector(T x, T y)
	{
		elements[0] = x;
		elements[1] = y;
	}

	template<typename T>
	inline const T& Vector<T, 2>::operator[](size_t pos) const
	{
		return elements.at(pos);
	}

	template<typename T>
	inline T& Vector<T, 2>::operator[](size_t pos)
	{
		return elements.at(pos);
	}

	template<typename T>
	inline const T& Vector<T, 2>::x() const { return elements[0]; }

	template<typename T>
	inline const T& Vector<T, 2>::y() const { return elements[1]; }

	template<typename T>
	inline T& Vector<T, 2>::x() { return elements[0]; }

	template<typename T>
	inline T& Vector<T, 2>::y() { return elements[1]; }

	template<typename T>
	inline T Vector<T, 2>::magnitude() const
	{
		return sqrt(Dot(*this));
	}

	template<typename T>
	inline T Vector<T, 2>::sqrMagnitude() const
	{
		return Dot(*this);
	}

	template<typename T>
	inline Vector<T, 2> Vector<T, 2>::normalized() const
	{
		T mag = magnitude();
		if (mag < epsilon)
			return { 0 };

		return *this / mag;
	}

	template<typename T>
	inline T Vector<T, 2>::Dot(const Vector& vec) const
	{
		return elements[0] * vec.elements[0] + elements[1] * vec.elements[1];
	}

	template<typename T>
	inline Vector<T, 2> Vector<T, 2>::operator+(const Vector& vec) const
	{
		Vector res;
		res.elements[0] = elements[0] + vec.elements[0];
		res.elements[1] = elements[1] + vec.elements[1];
		return res;
	}

	template<typename T>
	inline Vector<T, 2> Vector<T, 2>::operator-(const Vector& vec) const
	{
		Vector res;
		res.elements[0] = elements[0] - vec.elements[0];
		res.elements[1] = elements[1] - vec.elements[1];
		return res;
	}

	template<typename T>
	inline Vector<T, 2> Vector<T, 2>::operator*(T a) const
	{
		Vector res;
		res.elements[0] = elements[0] * a;
		res.elements[1] = elements[1] * a;
		return res;
	}

	template<typename T>
	inline Vector<T, 2> Vector<T, 2>::operator/(T a) const
	{
		if (Utility::Abs(a) < epsilon)
			return { 0 };

		Vector res;
		res.elements[0] = elements[0] / a;
		res.elements[1] = elements[1] / a;
		return res;
	}

	inline Vector<int, 2>::Vector(int x)
	{
		elements.fill(x);
	}

	inline Vector<int, 2>::Vector(int x, int y)
	{
		elements[0] = x;
		elements[1] = y;
	}

	inline const int& Vector<int, 2>::operator[](size_t pos) const
	{
		return elements.at(pos);
	}

	inline int& Vector<int, 2>::operator[](size_t pos)
	{
		return elements.at(pos);
	}

	inline const int& Vector<int, 2>::x() const
	{
		return elements[0];
	}

	inline const int& Vector<int, 2>::y() const
	{
		return elements[1];
	}

	inline int& Vector<int, 2>::x()
	{
		return elements[0];
	}

	inline int& Vector<int, 2>::y()
	{
		return elements[1];
	}

	inline int Vector<int, 2>::Dot(const Vector& vec) const
	{
		return elements[0] * vec.elements[0] + elements[1] * vec.elements[1];
	}

	inline Vector<int, 2> Vector<int, 2>::operator+(const Vector& vec) const
	{
		Vector res;
		res.elements[0] = elements[0] + vec.elements[0];
		res.elements[1] = elements[1] + vec.elements[1];
		return res;
	}

	inline Vector<int, 2> Vector<int, 2>::operator-(const Vector& vec) const
	{
		Vector res;
		res.elements[0] = elements[0] - vec.elements[0];
		res.elements[1] = elements[1] - vec.elements[1];
		return res;
	}

	inline Vector<int, 2> Vector<int, 2>::operator*(int a) const
	{
		Vector res;
		res.elements[0] = elements[0] * a;
		res.elements[1] = elements[1] * a;
		return res;
	}

	inline Vector<int, 2> Vector<int, 2>::operator/(int a) const
	{
		if (a == 0)
			return {0};

		Vector res;
		res.elements[0] = elements[0] / a;
		res.elements[1] = elements[1] / a;
		return res;
	}
}