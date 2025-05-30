/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename Derived> requires IsMatrixExpression<Derived>
    class MatrixBase
    {
        using Scalar = Traits<Derived>::Scalar;
        static constexpr bool IsVector = Traits<Derived>::Rows == 1 || Traits<Derived>::Columns == 1;
        static constexpr bool IsSquare = Traits<Derived>::Rows == Traits<Derived>::Columns;
        static constexpr bool IsLeftValue = HasFlag<Derived, XprFlag::LeftValue>;

    public:
        const Derived& Expression() const noexcept { return *static_cast<Derived const*>(this); }
        Derived& Expression() noexcept { return *static_cast<Derived*>(this); }

        static constexpr std::size_t Rows() noexcept { return Traits<Derived>::Rows; }
        static constexpr std::size_t Columns() noexcept { return Traits<Derived>::Columns; }

        static CWiseNullaryOp<ScalarIdentityOp<Scalar>, Derived> Identity()
        {
            return CWiseNullaryOp<ScalarIdentityOp<Scalar>, Derived>{{}};
        }

        static CWiseNullaryOp<ScalarConstantOp<Scalar>, Derived> Constant(Scalar scalar)
        {
            return CWiseNullaryOp<ScalarConstantOp<Scalar>, Derived>{
                ScalarConstantOp<Scalar>{scalar}};
        }

        static CWiseNullaryOp<ScalarConstantOp<Scalar>, Derived> Zero()
        {
            return Constant(static_cast<Scalar>(0));
        }

        template<typename OtherDerived>
        CWiseBinaryOp<ScalarAddOp<Scalar>, Derived, OtherDerived>
            operator+(const MatrixBase<OtherDerived>& other) const
        {
            return { Expression(), other.Expression() };
        }

        template<typename OtherDerived>
        CWiseBinaryOp<ScalarSubtractOp<Scalar>, Derived, OtherDerived>
            operator-(const MatrixBase<OtherDerived>& other) const
        {
            return { Expression(), other.Expression() };
        }

        template<typename OtherDerived>
        Derived& operator+=(const MatrixBase<OtherDerived>& other)
        {
            CallAssignment(this->Expression(), other.Expression(), AddAssignOp<Scalar>());
            return Expression();
        }

        template<typename OtherDerived>
        Derived& operator-=(const MatrixBase<OtherDerived>& other)
        {
            CallAssignment(this->Expression(), other.Expression(), SubtractAssignOp<Scalar>());
            return Expression();
        }

        template<typename OtherDerived>
        CWiseBinaryOp<ScalarDivideOp<Scalar>, Derived, OtherDerived>
            operator/(const MatrixBase<OtherDerived>& other) const
        {
            return { Expression(), other.Expression() };
        }

        CWiseBinaryOp<ScalarDivideOp<Scalar>, Derived, CWiseNullaryOp<ScalarConstantOp<Scalar>, Derived>>
            operator/(Scalar scalar) const
        {
            return { Expression(), Constant(scalar) };
        }

        CWiseBinaryOp<ScalarProductOp<Scalar>, Derived, CWiseNullaryOp<ScalarConstantOp<Scalar>, Derived>>
            operator*(Scalar scalar) const
        {
            return { Expression(), Constant(scalar) };
        }

        template<typename OtherDerived>
        CWiseBinaryOp<ScalarProductOp<Scalar>, Derived, OtherDerived>
            CWiseProduct(const MatrixBase<OtherDerived>& other) const
        {
            return { Expression(), other.Expression() };
        }

        template<typename OtherDerived>
        Product<Derived, OtherDerived, ProductType::Default>
            operator*(const MatrixBase<OtherDerived>& other) const
        {
            return { Expression(), other.Expression() };
        }

        template<typename OtherDerived>
        Derived& operator*=(const MatrixBase<OtherDerived>& other)
        {
            Expression() = Expression() * other.Expression();
            return Expression();
        }

        Derived& operator*=(Scalar scalar)
        {
            Expression() = Expression() * scalar;
            return Expression();
        }

        template<typename OtherDerived>
        Product<Derived, OtherDerived, ProductType::Lazy> LazyProduct(const MatrixBase<OtherDerived>& other) const
        {
            return { Expression(), other.Expression() };
        }

        template<typename BinaryOp>
        [[nodiscard]] Scalar Reduce(BinaryOp functor) const
        {
            return Reduction<Derived, BinaryOp>{}(Expression(), functor);
        }

        [[nodiscard]] Scalar Sum() const
        {
            return Reduce(ScalarAddOp<Scalar>{});
        }

        template<typename OtherDerived>
        [[nodiscard]] Scalar Dot(const MatrixBase<OtherDerived>& other) const
        {
            return CWiseProduct(other).Sum();
        }

        template<typename OtherDerived> requires IsVector
        [[nodiscard]] CrossImpl<Derived, OtherDerived>::ReturnType Cross(const MatrixBase<OtherDerived>& other) const
        {
            return CrossImpl<Derived, OtherDerived>{}(Expression(), other.Expression());
        }

        Scalar Norm() const
        {
            // Frobenius范数（矩阵的欧几里得范数），对应向量在欧几里得空间中的直线距离
            return std::sqrt(Dot(Expression()));
        }

        Scalar SquaredNorm() const
        {
            return Dot(Expression());
        }

        void Normalize() requires IsLeftValue
        {
            Expression() = Expression() / Norm();
        }

        PlainMatrix<Derived> Normalized() const
        {
            return Expression() / Norm();
        }

        template<std::size_t BlockRows, std::size_t BlockCols>
        Math::Block<Derived, BlockRows, BlockCols> Block(std::size_t startRow, std::size_t startColumn)
        {
            return { Expression(), startRow, startColumn };
        }

        template<std::size_t BlockRows, std::size_t BlockCols>
        Math::Block<const Derived, BlockRows, BlockCols> Block(std::size_t startRow, std::size_t startColumn) const
        {
            return { Expression(), startRow, startColumn };
        }

        Math::Transpose<Derived> Transpose() const
        {
            return Math::Transpose<Derived>(Expression());
        }

        Math::Block<Derived, 1, Traits<Derived>::Columns> Row(std::size_t row)
        {
            return { Expression(), row, 0 };
        }

        Math::Block<const Derived, 1, Traits<Derived>::Columns> Row(std::size_t row) const
        {
            return { Expression(), row, 0 };
        }

        Math::Block<Derived, Traits<Derived>::Rows, 1> Column(std::size_t column)
        {
            return { Expression(), 0, column };
        }

        Math::Block<const Derived, Traits<Derived>::Rows, 1> Column(std::size_t column) const
        {
            return { Expression(), 0, column };
        }

        Scalar Determinant() const requires IsSquare
        {
            return DeterminantImpl<Derived, Traits<Derived>::Rows>{}(Expression());
        }

        Math::Inverse<Derived> Inverse() const requires IsSquare
        {
            return Math::Inverse<Derived>(Expression());
        }
    };
}
