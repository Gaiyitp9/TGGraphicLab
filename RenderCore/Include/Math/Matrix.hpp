/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
	template<typename Scalar_, int Rows_, int Columns_, StorageOption Option>
	struct Traits<Matrix<Scalar_, Rows_, Columns_, Option>>
	{
		using Scalar = Scalar_;
        static constexpr int	    Rows = Rows_;
        static constexpr int	    Columns = Columns_;
        static constexpr int	    Size = Rows * Columns;
        static constexpr XprFlag    Flags = (Option == StorageOption::RowMajor ? XprFlag::RowMajor : XprFlag::None) |
                XprFlag::DirectAccess | XprFlag::LinearAccess |
                (Rows == 1 || Columns == 1 ? XprFlag::Vector : XprFlag::None) |
                (Rows == Columns ? XprFlag::Square : XprFlag::None);
	};

	template<typename Scalar, int Rows, int Columns, StorageOption Option>
	class Matrix : public MatrixBase<Matrix<Scalar, Rows, Columns, Option>>
	{
        using Base = MatrixBase<Matrix<Scalar, Rows, Columns, Option>>;
        using Base::Expression;

	public:
		Matrix() = default;
		Matrix(const Matrix& other) { std::memcpy(m_storage, other.m_storage, Traits<Matrix>::Size * sizeof(Scalar)); }
        Matrix(Matrix&&) = delete;

        template<typename Derived>
        Matrix(const MatrixBase<Derived>& other)
        {
            CallAssignmentNoAlias(Expression(), other.Expression());
        }

		template<typename Derived>
		Matrix& operator=(const MatrixBase<Derived>& other)
		{
            CallAssignment(Expression(), other.Expression());
			return *this;
		}

        const Scalar* Data() const { return m_storage; }
        Scalar* Data() { return m_storage; }

		const Scalar& operator[](int index) const
		{
			return m_storage[index];
		}
		const Scalar& operator()(int row, int col) const
		{
			if constexpr (CheckFlag<Matrix>(XprFlag::RowMajor))
				return m_storage[col + row * Columns];
			else
				return m_storage[row + col * Rows];
		}
		Scalar& operator[](int index)
		{
			return m_storage[index];
		}
		Scalar& operator()(int row, int col)
		{
            if constexpr (CheckFlag<Matrix>(XprFlag::RowMajor))
				return m_storage[col + row * Columns];
            else
				return m_storage[row + col * Rows];
		}

	private:
		Scalar m_storage[Traits<Matrix>::Size]{};
	};

	template<typename Scalar, int Size>
	using Vector = Matrix<Scalar, Size, 1>;
	template<typename Scalar, int Size>
	using RowVector = Matrix<Scalar, 1, Size>;

	#define MATRIX_TYPEDEF(Type, TypeSuffix, Size, SizeSuffix)			\
	using Vector##SizeSuffix##TypeSuffix	= Vector<Type, Size>;		\
	using RowVector##SizeSuffix##TypeSuffix = RowVector<Type, Size>;	\
	using Matrix##SizeSuffix##TypeSuffix	= Matrix<Type, Size, Size>;

	#define MATRIX_ALL_SIZE_TYPEDEF(Type, TypeSuffix)	\
	MATRIX_TYPEDEF(Type, TypeSuffix, 2, 2)				\
	MATRIX_TYPEDEF(Type, TypeSuffix, 3, 3)				\
	MATRIX_TYPEDEF(Type, TypeSuffix, 4, 4)

	MATRIX_ALL_SIZE_TYPEDEF(float, f)
	MATRIX_ALL_SIZE_TYPEDEF(double, d)
	MATRIX_ALL_SIZE_TYPEDEF(int, i)

    // 矩阵求值器
    template<typename Scalar, int Rows, int Columns, StorageOption Option>
    class Evaluator<Matrix<Scalar, Rows, Columns, Option>>
    {
    public:
        using XprType = Matrix<Scalar, Rows, Columns, Option>;
        using CoeffType= Traits<XprType>::Scalar;

        explicit Evaluator(const XprType& mat) : m_data(mat.Data()) {}

        [[nodiscard]] CoeffType Coefficient(int index) const
        {
            return m_data[index];
        }
        [[nodiscard]] CoeffType Coefficient(int row, int col) const
        {
            if constexpr (CheckFlag<XprType>(XprFlag::RowMajor))
                return m_data[row * Traits<XprType>::Columns + col];
            else
                return m_data[row + col * Traits<XprType>::Rows];
        }
        CoeffType& CoefficientRef(int index)
        {
            return const_cast<CoeffType*>(m_data)[index];
        }
        CoeffType& CoefficientRef(int row, int col)
        {
            if constexpr (CheckFlag<XprType>(XprFlag::RowMajor))
                return const_cast<CoeffType*>(m_data)[row * Traits<XprType>::Columns + col];
            else
                return const_cast<CoeffType*>(m_data)[row + col * Traits<XprType>::Rows];
        }

    private:
        CoeffType const* m_data;
    };
}
