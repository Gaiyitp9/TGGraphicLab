/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{
	template<typename Scalar, int Rows, int Cols, StorageOption Option>
	class Matrix
	{
	public:
		constexpr static bool IsRowMajor = Option == StorageOption::RowMajor;
		constexpr static bool IsVectorAtCompileTime = Rows == 1 || Cols == 1;
		constexpr static int SizeAtCompileTime = (Rows == Dynamic || Cols == Dynamic) ? Dynamic : Rows * Cols;
		constexpr static int Alignment = Rows == Dynamic || Cols == Dynamic ? DEFAULT_ALIGN_BYTES : default_alignment<Scalar, SizeAtCompileTime>;
		using PacketType = best_packet<Scalar, SizeAtCompileTime>;
		constexpr static bool IsAligned = Alignment > 0;

	public:
		Matrix() = default;
		Matrix(const Matrix& other) : m_storage(other.m_storage) {}

		template<typename OtherDerived>
		Matrix& operator=(const Matrix& other)
		{
			AssignmentOp<Matrix>::Run(*this, other);
			return *this;
		}

	public:
		constexpr int rows() const { return m_storage.rows(); }
		constexpr int cols() const { return m_storage.cols(); }
		const Scalar* data() const { return m_storage.data(); }
		Scalar* data() { return m_storage.data(); }

		const Scalar& x() const
		{
			static_assert(IsVectorAtCompileTime, "x() is only for vectors.");
			return m_storage[0];
		}
		const Scalar& y() const
		{
			static_assert(IsVectorAtCompileTime, "y() is only for vectors.");
			return m_storage[1];
		}
		const Scalar& z() const
		{
			static_assert(IsVectorAtCompileTime, "z() is only for vectors.");
			return m_storage[2];
		}
		const Scalar& w() const
		{
			static_assert(IsVectorAtCompileTime, "w() is only for vectors.");
			return m_storage[3];
		}

		Scalar& x()
		{
			static_assert(IsVectorAtCompileTime, "x() is only for vectors.");
			return m_storage[0];
		}
		Scalar& y()
		{
			static_assert(IsVectorAtCompileTime, "y() is only for vectors.");
			return m_storage[1];
		}
		Scalar& z()
		{
			static_assert(IsVectorAtCompileTime, "z() is only for vectors.");
			return m_storage[2];
		}
		Scalar& w()
		{
			static_assert(IsVectorAtCompileTime, "w() is only for vectors.");
			return m_storage[3];
		}

		const Scalar& operator[](int index) const
		{
			return m_storage[index];
		}
		const Scalar& operator()(int row, int col) const
		{
			if constexpr (IsRowMajor)
				return m_storage[col + row * m_storage.cols()];
			else
				return m_storage[row + col * m_storage.rows()];
		}
		Scalar& operator[](int index)
		{
			return m_storage[index];
		}
		Scalar& operator()(int row, int col)
		{
			if constexpr (IsRowMajor)
				return m_storage[col + row * m_storage.cols()];
			else
				return m_storage[row + col * m_storage.rows()];
		}

		Scalar magnitude() const
		{
			static_assert(IsVectorAtCompileTime, "Magnitude is only for vectors.");
			return 0;
		}

		Scalar sqrMagnitude() const
		{
			static_assert(IsVectorAtCompileTime, "sqrMagnitude is only for vectors.");
			return 0;
		}

	public:
		Matrix operator+(const Matrix& other) const
		{
			return CwiseBinaryOp<Matrix, ScalarSumOp<Scalar>>::Run(*this, other);
		}

		Matrix operator-(const Matrix& other) const
		{
			return CwiseBinaryOp<Matrix, ScalarSubOp<Scalar>>::Run(*this, other);
		}

		Matrix operator*(const Matrix& other) const
		{
			return CwiseBinaryOp<Matrix, ScalarProductOp<Scalar>>::Run(*this, other);
		}

		Scalar Dot(const Matrix& other) const
		{
			Scalar dot = 0;
			return dot;
		}

	private:
		Storage<Scalar, SizeAtCompileTime, Rows, Cols> m_storage;
	};

	#define MATRIX_TYPEDEF(Type, TypeSuffix, Size, SizeSuffix)			\
	using Vector##SizeSuffix##TypeSuffix	= Matrix<Type, Size, 1>;	\
	using RowVector##SizeSuffix##TypeSuffix = Matrix<Type, 1, Size>;	\
	using Matrix##SizeSuffix##TypeSuffix	= Matrix<Type, Size, Size>;

	#define MATRIX_ALLSIZE_TYPEDEF(Type, TypeSuffix)	\
	MATRIX_TYPEDEF(Type, TypeSuffix, 2, 2)				\
	MATRIX_TYPEDEF(Type, TypeSuffix, 3, 3)				\
	MATRIX_TYPEDEF(Type, TypeSuffix, 4, 4)				\
	MATRIX_TYPEDEF(Type, TypeSuffix, Dynamic, X)		

	MATRIX_ALLSIZE_TYPEDEF(float, f)
	MATRIX_ALLSIZE_TYPEDEF(double, d)
	MATRIX_ALLSIZE_TYPEDEF(int, i)
}