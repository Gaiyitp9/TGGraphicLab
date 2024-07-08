/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
	template<typename Scalar_, std::size_t Rows_, std::size_t Columns_, StorageOrder Order>
	struct Traits<Matrix<Scalar_, Rows_, Columns_, Order>>
	{
		using Scalar = Scalar_;
        static constexpr std::size_t	Rows = Rows_;
        static constexpr std::size_t	Columns = Columns_;
        static constexpr std::size_t	Size = Rows * Columns;
        static constexpr XprFlag        Flags = (Order == StorageOrder::RowMajor ? XprFlag::RowMajor : XprFlag::None) |
                XprFlag::LeftValue | XprFlag::LinearAccess;
	};

	template<typename Scalar, std::size_t Rows, std::size_t Columns, StorageOrder Order>
	class Matrix final : public MatrixBase<Matrix<Scalar, Rows, Columns, Order>>
	{
		using Base = MatrixBase<Matrix>;

	public:
		Matrix() = default;

		// 构造时才会调用，可以保证不存在aliasing问题
        template<typename Derived>
        Matrix(const MatrixBase<Derived>& other)
        {
            CallAssignmentNoAlias(this->Expression(), other.Expression(), AssignOp<Scalar>{});
        }

		template<typename Derived>
		Matrix& operator=(const MatrixBase<Derived>& other)
        {
        	Base::operator=(other);
	        return *this;
        }

		const Scalar& operator[](std::size_t index) const
		{
			return m_storage[index];
		}
		const Scalar& operator()(std::size_t row, std::size_t column) const
		{
			if constexpr (HasFlag<Matrix, XprFlag::RowMajor>)
				return m_storage[column + row * Columns];
			else
				return m_storage[row + column * Rows];
		}
		Scalar& operator[](std::size_t index)
		{
			return m_storage[index];
		}
		Scalar& operator()(std::size_t row, std::size_t column)
		{
            if constexpr (HasFlag<Matrix, XprFlag::RowMajor>)
				return m_storage[column + row * Columns];
            else
				return m_storage[row + column * Rows];
		}

	private:
		Scalar m_storage[Rows * Columns]{};

        friend class Evaluator<Matrix>;
	};

	template<typename Scalar, std::size_t Size> using Vector = Matrix<Scalar, Size, 1>;
	template<typename Scalar, std::size_t Size> using RowVector = Matrix<Scalar, 1, Size>;

	using Vector2f = Vector<float, 2>;
	using Vector3f = Vector<float, 3>;
	using Vector4f = Vector<float, 4>;
	using RowVector2f = RowVector<float, 2>;
	using RowVector3f = RowVector<float, 3>;
	using RowVector4f = RowVector<float, 4>;
	using Matrix2f = Matrix<float, 2, 2>;
	using Matrix3f = Matrix<float, 3, 3>;
	using Matrix4f = Matrix<float, 4, 4>;

	using Vector2d = Vector<double, 2>;
	using Vector3d = Vector<double, 3>;
	using Vector4d = Vector<double, 4>;
	using RowVector2d = RowVector<double, 2>;
	using RowVector3d = RowVector<double, 3>;
	using RowVector4d = RowVector<double, 4>;
	using Matrix2d = Matrix<double, 2, 2>;
	using Matrix3d = Matrix<double, 3, 3>;
	using Matrix4d = Matrix<double, 4, 4>;

    // 矩阵求值器
    template<typename Scalar, std::size_t Rows, std::size_t Columns, StorageOrder Option>
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
        [[nodiscard]] CoeffType Coefficient(std::size_t row, std::size_t column) const
        {
            if constexpr (HasFlag<XprType, XprFlag::RowMajor>)
                return m_data[row * Traits<XprType>::Columns + column];
            else
                return m_data[row + column * Traits<XprType>::Rows];
        }
        CoeffType& CoefficientRef(std::size_t index)
        {
            return const_cast<CoeffType*>(m_data)[index];
        }
        CoeffType& CoefficientRef(std::size_t row, std::size_t column)
        {
            if constexpr (HasFlag<XprType, XprFlag::RowMajor>)
                return const_cast<CoeffType*>(m_data)[row * Traits<XprType>::Columns + column];
            else
                return const_cast<CoeffType*>(m_data)[row + column * Traits<XprType>::Rows];
        }

    private:
        CoeffType const* m_data;
    };
}
