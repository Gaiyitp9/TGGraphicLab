/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "MatrixBase.hpp"
#include "Assignment.hpp"

namespace TG::Math
{
	template<typename ScalarT, std::size_t RowsT, std::size_t ColumnsT, StorageOrder Order>
	struct Traits<Matrix<ScalarT, RowsT, ColumnsT, Order>>
	{
		using Scalar = ScalarT;
        static constexpr std::size_t	Rows = RowsT;
        static constexpr std::size_t	Columns = ColumnsT;
        static constexpr std::size_t	Size = Rows * Columns;
        static constexpr XprFlag        Flags = (Order == StorageOrder::RowMajor ? XprFlag::RowMajor : XprFlag::None) |
			XprFlag::LeftValue | XprFlag::LinearAccess;
	};

	template<typename Scalar, std::size_t Rows, std::size_t Columns, StorageOrder Order>
	class Matrix final : public MatrixBase<Matrix<Scalar, Rows, Columns, Order>>
	{
	public:
		Matrix() = default;

		// 构造时才会调用，不存在aliasing问题
        template<typename Derived>
        Matrix(const MatrixBase<Derived>& other)
        {
            CallAssignmentNoAlias(this->Expression(), other.Expression());
        }

		template<typename Derived>
		Matrix& operator=(const MatrixBase<Derived>& other)
        {
        	CallAssignment(this->Expression(), other.Expression());
	        return *this;
        }

		Scalar const* Data() const noexcept { return m_storage; }

		Scalar operator[](std::size_t index) const
		{
			return m_storage[index];
		}
		Scalar operator()(std::size_t row, std::size_t column) const
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

    template<typename Scalar, std::size_t Rows, std::size_t Columns, StorageOrder Order, bool IsConst>
    class Evaluator<Matrix<Scalar, Rows, Columns, Order>, IsConst>
    {
        using Xpr = Matrix<Scalar, Rows, Columns, Order>;
    	using InternalXpr = std::conditional_t<IsConst, const Xpr, Xpr>;

    public:
        explicit Evaluator(InternalXpr& matrix) : m_matrix(matrix) {}

        [[nodiscard]] Scalar Entry(std::size_t index) const
        {
            return m_matrix[index];
        }

        [[nodiscard]] Scalar Entry(std::size_t row, std::size_t column) const
        {
            return m_matrix(row, column);
        }

    	Scalar& Entry(std::size_t index) requires !IsConst
        {
        	return m_matrix[index];
        }

    	Scalar& Entry(std::size_t row, std::size_t column) requires !IsConst
        {
        	return m_matrix(row, column);
        }

    private:
        InternalXpr& m_matrix;
    };
}
