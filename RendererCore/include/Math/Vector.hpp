/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Simd.hpp"
#include "MathUtil.hpp"

namespace LCH::Math
{
	template<typename T, size_t Dimension> requires type_and_dimension<T, Dimension>
	class Vector
	{
		using simd = simd_trait<T, typename SimdInstruction<T, Dimension>::type>;
		static constexpr size_t loop = Dimension / simd::DataCount;
		static constexpr size_t remainder = Dimension % simd::DataCount;
	public:
		Vector(T x = {});

		const T& operator[](size_t index) const;
		T& operator[](size_t index);

		Vector operator+(const Vector& vec) const;

	private:
		aligned_array<T, Dimension, simd::Alignment> elements;
	};

	template<typename T> requires type_and_dimension<T, 4>
	class Vector<T, 4>
	{
		using simd = simd_trait<T, typename SimdInstruction<T, 4>::type>;
	public:
		Vector(T x = {});
		Vector(T x, T y, T z, T w);

	public:
		const T& operator[](size_t pos) const;
		T& operator[](size_t pos);

		const T& x() const;
		const T& y() const;
		const T& z() const;
		const T& w() const;
		T& x();
		T& y();
		T& z();
		T& w();

		T magnitude() const;
		T sqrMagnitude() const;
		Vector normalized() const;

	public:
		T Dot(const Vector& vec) const;

		Vector operator+(const Vector& vec) const;
		Vector operator-(const Vector& vec) const;
		Vector operator*(T a) const;
		Vector operator/(T a) const;

	private:
		aligned_array<T, 4, simd::Alignment> elements;
	};

	template<typename T> requires type_and_dimension<T, 3>
	class Vector<T, 3>
	{
	public:
		Vector(T x = {});
		Vector(T x, T y, T z);

	public:
		const T& operator[](size_t pos) const;
		T& operator[](size_t pos);

		const T& x() const;
		const T& y() const;
		const T& z() const;
		T& x();
		T& y();
		T& z();

		T magnitude() const;
		T sqrMagnitude() const;
		Vector normalized() const;

	public:
		T Dot(const Vector& vec) const;
		Vector Cross(const Vector& vec) const;

		Vector operator+(const Vector& vec) const;
		Vector operator-(const Vector& vec) const;
		Vector operator*(T a) const;
		Vector operator/(T a) const;

	private:
		std::array<T, 3> elements;
	};

	template<typename T> requires type_and_dimension<T, 2>
	class Vector<T, 2>
	{
	public:
		Vector(T x = {});
		Vector(T x, T y);

	public:
		const T& operator[](size_t pos) const;
		T& operator[](size_t pos);

		const T& x() const;
		const T& y() const;
		T& x();
		T& y();

		T magnitude() const;
		T sqrMagnitude() const;
		Vector normalized() const;

	public:
		T Dot(const Vector& vec) const;

		Vector operator+(const Vector& vec) const;
		Vector operator-(const Vector& vec) const;
		Vector operator*(T a) const;
		Vector operator/(T a) const;

	private:
		std::array<T, 2> elements;
	};

	using Vector4f = Vector<float, 4>;
	using Vector4d = Vector<double, 4>;
	using Vector4i = Vector<int, 4>;
	using Vector3f = Vector<float, 3>;
	using Vector3d = Vector<double, 3>;
	using Vector3i = Vector<int, 3>;
	using Vector2f = Vector<float, 2>;
	using Vector2d = Vector<double, 2>;
	using Vector2i = Vector<int, 2>;
}

#include "Implementation/Vector.inl"