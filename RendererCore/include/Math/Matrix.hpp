/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
	template<typename ScalarT, int Rows, int Cols, StorageOption Option>
	struct traits<Matrix<ScalarT, Rows, Cols, Option>>
	{
		using Scalar = ScalarT;
		constexpr static int	RowsAtCompileTime = Rows;
		constexpr static int	ColsAtCompileTime = Cols;
		constexpr static bool	IsRowMajor = Option == StorageOption::RowMajor;
		constexpr static bool	IsDynamic = Rows == DYNAMIC || Cols == DYNAMIC;
		constexpr static bool	IsVectorAtCompileTime = Rows == 1 || Cols == 1;
		constexpr static bool	IsSquareAtCompileTime = Rows == Cols;
		constexpr static int	SizeAtCompileTime = IsDynamic ? DYNAMIC : Rows * Cols;
	};

	template<typename Scalar, int Rows, int Cols, StorageOption Option>
	class Matrix
	{
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
			static_assert(traits<Matrix>::IsVectorAtCompileTime, "x() is only for vectors.");
			return m_storage[0];
		}
		const Scalar& y() const
		{
			static_assert(traits<Matrix>::IsVectorAtCompileTime, "y() is only for vectors.");
			return m_storage[1];
		}
		const Scalar& z() const
		{
			static_assert(traits<Matrix>::IsVectorAtCompileTime, "z() is only for vectors.");
			return m_storage[2];
		}
		const Scalar& w() const
		{
			static_assert(traits<Matrix>::IsVectorAtCompileTime, "w() is only for vectors.");
			return m_storage[3];
		}

		Scalar& x()
		{
			static_assert(traits<Matrix>::IsVectorAtCompileTime, "x() is only for vectors.");
			return m_storage[0];
		}
		Scalar& y()
		{
			static_assert(traits<Matrix>::IsVectorAtCompileTime, "y() is only for vectors.");
			return m_storage[1];
		}
		Scalar& z()
		{
			static_assert(traits<Matrix>::IsVectorAtCompileTime, "z() is only for vectors.");
			return m_storage[2];
		}
		Scalar& w()
		{
			static_assert(traits<Matrix>::IsVectorAtCompileTime, "w() is only for vectors.");
			return m_storage[3];
		}

		const Scalar& operator[](int index) const
		{
			return m_storage[index];
		}
		const Scalar& operator()(int row, int col) const
		{
			if constexpr (traits<Matrix>::IsRowMajor)
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
			if constexpr (traits<Matrix>::IsRowMajor)
				return m_storage[col + row * m_storage.cols()];
			else
				return m_storage[row + col * m_storage.rows()];
		}

		Scalar magnitude() const
		{
			if constexpr (!traits<Matrix>::IsDynamic)
				static_assert(traits<Matrix>::IsVectorAtCompileTime, "Magnitude is only for vectors.");
			else if (rows() != 1 && cols() != 1)
				Debug::LogLine(L"magnitude is only for vectors.");

			return 0;
		}

		Scalar sqrMagnitude() const
		{
			if constexpr (!traits<Matrix>::IsDynamic)
				static_assert(traits<Matrix>::IsVectorAtCompileTime, "sqrMagnitude is only for vectors.");
			else if (rows() != 1 && cols() != 1)
				Debug::LogLine(L"sqrMagnitude is only for vectors.");

			return 0;
		}

		Scalar determinant() const
		{
			if constexpr (!traits<Matrix>::IsDynamic)
				static_assert(traits<Matrix>::IsSquareAtCompileTime, "determinant is only for square matrix.");
			else if (rows() != cols())
				Debug::LogLine(L"determinant is only for square matrix.");

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

		// 矩阵乘法(注意满足矩阵的列与other矩阵的行数要相等)
		template<typename OtherMatrix> 
		typename MatricesMultipleTraits<Matrix, OtherMatrix>::ReturnType 
			operator*(const OtherMatrix& other) const
		{
			return Multiple<Matrix, OtherMatrix>::Run(*this, other);
		}

		// 逐元素乘法
		Matrix CWiseMultiple(const Matrix& other) const
		{
			return CwiseBinaryOp<Matrix, ScalarProductOp<Scalar>>::Run(*this, other);
		}

		Scalar Dot(const Matrix& other) const
		{

		}

		// 向量叉乘
		Matrix Cross(const Matrix& other) const
		{

		}

		// 矩阵转置
		Matrix Transpose() const
		{

		}

		// 矩阵的逆
		Matrix Inverse() const
		{

		}

	private:
		Storage<Scalar, 
			traits<Matrix>::SizeAtCompileTime, 
			traits<Matrix>::RowsAtCompileTime, 
			traits<Matrix>::ColsAtCompileTime
		> m_storage;
	};

	#define MATRIX_TYPEDEF(Type, TypeSuffix, Size, SizeSuffix)			\
	using Vector##SizeSuffix##TypeSuffix	= Matrix<Type, Size, 1>;	\
	using RowVector##SizeSuffix##TypeSuffix = Matrix<Type, 1, Size>;	\
	using Matrix##SizeSuffix##TypeSuffix	= Matrix<Type, Size, Size>;

	#define MATRIX_ALLSIZE_TYPEDEF(Type, TypeSuffix)	\
	MATRIX_TYPEDEF(Type, TypeSuffix, 2, 2)				\
	MATRIX_TYPEDEF(Type, TypeSuffix, 3, 3)				\
	MATRIX_TYPEDEF(Type, TypeSuffix, 4, 4)				\
	MATRIX_TYPEDEF(Type, TypeSuffix, DYNAMIC, X)		

	MATRIX_ALLSIZE_TYPEDEF(float, f)
	MATRIX_ALLSIZE_TYPEDEF(double, d)
	MATRIX_ALLSIZE_TYPEDEF(int, i)
}