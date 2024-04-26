/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
	template<typename Scalar_, std::size_t Rows_, std::size_t Columns_, StorageOption Option>
	struct Traits<Matrix<Scalar_, Rows_, Columns_, Option>>
	{
		using Scalar = Scalar_;
        static constexpr std::size_t	Rows = Rows_;
        static constexpr std::size_t	Columns = Columns_;
        static constexpr std::size_t	Size = Rows * Columns;
        static constexpr XprFlag        Flags = (Option == StorageOption::RowMajor ? XprFlag::RowMajor : XprFlag::None) |
                XprFlag::LeftValue | XprFlag::LinearAccess |
                (Rows == 1 || Columns == 1 ? XprFlag::Vector : XprFlag::None) |
                (Rows == Columns ? XprFlag::Square : XprFlag::None);
	};

	template<typename Scalar, std::size_t Rows, std::size_t Columns, StorageOption Option>
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

        using Base::operator=;

		const Scalar& operator[](std::size_t index) const
		{
			return m_storage[index];
		}
		const Scalar& operator()(std::size_t row, std::size_t col) const
		{
			if constexpr (CheckFlag<Matrix>(XprFlag::RowMajor))
				return m_storage[col + row * Columns];
			else
				return m_storage[row + col * Rows];
		}
		Scalar& operator[](std::size_t index)
		{
			return m_storage[index];
		}
		Scalar& operator()(std::size_t row, std::size_t col)
		{
            if constexpr (CheckFlag<Matrix>(XprFlag::RowMajor))
				return m_storage[col + row * Columns];
            else
				return m_storage[row + col * Rows];
		}

	private:
		Scalar m_storage[Traits<Matrix>::Size]{};

        friend class Evaluator<Matrix>;
	};

	template<typename Scalar, std::size_t Size> using Vector = Matrix<Scalar, Size, 1>;
	template<typename Scalar, std::size_t Size> using RowVector = Matrix<Scalar, 1, Size>;

	#define MATRIX_TYPEDEF(Type, TypeSuffix, Size)			\
	using Vector##Size##TypeSuffix	= Vector<Type, Size>;		\
	using RowVector##Size##TypeSuffix = RowVector<Type, Size>;	\
	using Matrix##Size##TypeSuffix	= Matrix<Type, Size, Size>;

	#define MATRIX_ALL_SIZE_TYPEDEF(Type, TypeSuffix)	\
	MATRIX_TYPEDEF(Type, TypeSuffix, 2)				\
	MATRIX_TYPEDEF(Type, TypeSuffix, 3)				\
	MATRIX_TYPEDEF(Type, TypeSuffix, 4)

	MATRIX_ALL_SIZE_TYPEDEF(float, f)
	MATRIX_ALL_SIZE_TYPEDEF(double, d)
	MATRIX_ALL_SIZE_TYPEDEF(int, i)

    // 矩阵求值器
    template<typename Scalar, std::size_t Rows, std::size_t Columns, StorageOption Option>
    class Evaluator<Matrix<Scalar, Rows, Columns, Option>>
    {
    public:
        using XprType = Matrix<Scalar, Rows, Columns, Option>;
        using CoeffType= Traits<XprType>::Scalar;

        explicit Evaluator(const XprType& mat) : m_data(mat.m_storage) {}

        [[nodiscard]] CoeffType Coefficient(std::size_t index) const
        {
            return m_data[index];
        }
        [[nodiscard]] CoeffType Coefficient(std::size_t row, std::size_t col) const
        {
            if constexpr (CheckFlag<XprType>(XprFlag::RowMajor))
                return m_data[row * Traits<XprType>::Columns + col];
            else
                return m_data[row + col * Traits<XprType>::Rows];
        }
        CoeffType& CoefficientRef(std::size_t index)
        {
            return const_cast<CoeffType*>(m_data)[index];
        }
        CoeffType& CoefficientRef(std::size_t row, std::size_t col)
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
