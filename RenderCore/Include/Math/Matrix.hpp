/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
	template<typename T, int R, int C, StorageOption Option>
	struct traits<Matrix<T, R, C, Option>>
	{
		using Scalar = T;
        static constexpr int	Rows = R;
        static constexpr int	Columns = C;
        static constexpr bool	IsRowMajor = Option == StorageOption::RowMajor;
        static constexpr bool	IsVector = Rows == 1 || Columns == 1;
        static constexpr bool	IsSquare = Rows == Columns;
        static constexpr int	Size = Rows * Columns;
	};

	template<typename Scalar, int Rows, int Cols, StorageOption Option>
	class Matrix
	{
	public:
		Matrix() { std::memset(m_storage.data(), 0, m_storage.size() * sizeof(Scalar)); }
		Matrix(const Matrix& other) : m_storage(other.m_storage) {}

		template<typename OtherDerived>
		Matrix& operator=(const Matrix& other)
		{
			AssignmentOp<Matrix>::Run(*this, other);
			return *this;
		}

	public:
		[[nodiscard]] constexpr int rows() const { return traits<Matrix>::Rows; }
		[[nodiscard]] constexpr int cols() const { return traits<Matrix>::Columns; }
		[[nodiscard]] constexpr int size() const { return traits<Matrix>::Size; }

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

		// 矩阵转置
		Matrix transpose() const
		{
			Matrix<Scalar, Cols, Rows, Option> t;
			for (int i = 0; i < rows(); ++i)
			{
				for (int j = 0; j < cols(); ++i)
				{
					t(j, i) = (*this)(i, j);
				}
			}
			return t;
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

		Matrix operator/(const Scalar& factor) const
		{
			if (Abs(factor) < NumTraits<Scalar>::Epsilon)
				return *this;

			Matrix factorMatrix;
			for (int i = 0; i < rows(); ++i)
			{
				for (int j = 0; j < cols(); ++j)
				{
					factorMatrix(i, j) = factor;
				}
			}
			return CwiseBinaryOp<Matrix, ScalarDivideOp<Scalar>>::Run(*this, factorMatrix);
		}

		Matrix operator*(const Scalar& factor) const
		{
			Matrix factorMatrix;
			for (int i = 0; i < rows(); ++i)
			{
				for (int j = 0; j < cols(); ++j)
				{
					factorMatrix(i, j) = factor;
				}
			}
			return CwiseBinaryOp<Matrix, ScalarProductOp<Scalar>>::Run(*this, factorMatrix);
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
			return DotOp<Matrix>::Run(*this, other);
		}

	private:
		Scalar m_storage[traits<Matrix>::Size];
	};

	// 矩阵乘以一个系数
	template<typename Scalar, int Rows, int Cols, StorageOption Option>
	Matrix<Scalar, Rows, Cols, Option> operator*(const Scalar& factor, const Matrix<Scalar, Rows, Cols, Option>& matrix)
	{
		return matrix * factor;
	}

	template<typename Scalar, int Size>
	using Vector = Matrix<Scalar, Size, 1>;
	template<typename Scalar, int Size>
	using RowVector = Matrix<Scalar, 1, Size>;

	#define MATRIX_TYPEDEF(Type, TypeSuffix, Size, SizeSuffix)			\
	using Vector##SizeSuffix##TypeSuffix	= Vector<Type, Size>;		\
	using RowVector##SizeSuffix##TypeSuffix = RowVector<Type, Size>;	\
	using Matrix##SizeSuffix##TypeSuffix	= Matrix<Type, Size, Size>;

	#define MATRIX_ALLSIZE_TYPEDEF(Type, TypeSuffix)	\
	MATRIX_TYPEDEF(Type, TypeSuffix, 2, 2)				\
	MATRIX_TYPEDEF(Type, TypeSuffix, 3, 3)				\
	MATRIX_TYPEDEF(Type, TypeSuffix, 4, 4)

	MATRIX_ALLSIZE_TYPEDEF(float, f)
	MATRIX_ALLSIZE_TYPEDEF(double, d)
	MATRIX_ALLSIZE_TYPEDEF(int, i)
}