/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
	template<typename Scalar, int Rows, int Cols, StorageOption Option>
	class Matrix
	{
	public:
		constexpr static int RowsAtCompileTime = Rows;
		constexpr static int ColsAtCompileTime = Cols;
		constexpr static bool IsRowMajor = Option == StorageOption::RowMajor;
		constexpr static bool IsDynamic = Rows == Dynamic || Cols == Dynamic;
		constexpr static bool IsVectorAtCompileTime = Rows == 1 || Cols == 1;
		constexpr static bool IsScalarAtCompileTime = Rows == 1 && Cols == 1;
		constexpr static bool IsSquareAtCompileTime = Rows == Cols;
		constexpr static int SizeAtCompileTime = IsDynamic ? Dynamic : Rows * Cols;
		constexpr static int Alignment = Rows == Dynamic || Cols == Dynamic ? DEFAULT_ALIGN_BYTES : default_alignment<Scalar, SizeAtCompileTime>;
		constexpr static bool IsAligned = Alignment > 0;
		using PacketType = best_packet<Scalar, SizeAtCompileTime>;

	public:
		Matrix() = default;
		Matrix(const Matrix& other) : m_storage(other.m_storage) {}

		template<typename OtherDerived>
		Matrix& operator=(const Matrix& other)
		{
			AssignmentOp<Matrix>::Run(*this, other);
			return *this;
		}

		// 1X1矩阵隐式转为标量
		operator Scalar()
		{
			if constexpr (!IsDynamic)
				static_assert(IsScalarAtCompileTime, "Scalar implicit type cast function only works for 1X1 dimension matrix");
			else if (rows() != 1 && cols != 1)
				Debug::LogLine(L"The matrix rows and cols must both equal 1");
			return m_storage[0];
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
			if constexpr (!IsDynamic)
				static_assert(IsVectorAtCompileTime, "Magnitude is only for vectors.");
			else if (rows() != 1 && cols() != 1)
				Debug::LogLine(L"magnitude is only for vectors.");

			return 0;
		}

		Scalar sqrMagnitude() const
		{
			if constexpr (!IsDynamic)
				static_assert(IsVectorAtCompileTime, "sqrMagnitude is only for vectors.");
			else if (rows() != 1 && cols() != 1)
				Debug::LogLine(L"sqrMagnitude is only for vectors.");

			return 0;
		}

		Scalar determinant() const
		{
			if constexpr (!IsDynamic)
				static_assert(IsSquareAtCompileTime, "determinant is only for square matrix.");
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

		// 矩阵乘法(包括点乘)
		// 注意满足矩阵的列与other矩阵的行数要相等
		Matrix operator*(const Matrix& other) const
		{

		}

		// 逐元素乘法
		Matrix CWiseMultiple(const Matrix& other) const
		{
			return CwiseBinaryOp<Matrix, ScalarProductOp<Scalar>>::Run(*this, other);
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
		Storage<Scalar, SizeAtCompileTime, RowsAtCompileTime, ColsAtCompileTime> m_storage;
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