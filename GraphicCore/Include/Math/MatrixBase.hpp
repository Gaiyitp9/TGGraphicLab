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

    public:
        template<typename OtherDerived>
        MatrixBase& operator=(const MatrixBase<OtherDerived>& other)
        {
            CallAssignment(Expression(), other.Expression());
            return *this;
        }

        const Derived& Expression() const noexcept { return *static_cast<Derived const*>(this); }
        Derived& Expression() noexcept { return *static_cast<Derived*>(this); }
        static constexpr std::size_t Rows() noexcept { return Traits<Derived>::Rows; }
        static constexpr std::size_t Columns() noexcept { return Traits<Derived>::Columns; }

        template<typename OtherDerived>
        CWiseBinaryOp<ScalarAddOp<Scalar>, Derived, OtherDerived> operator+(const MatrixBase<OtherDerived>& other) const
        {
            return {Expression(), other.Expression()};
        }

        template<typename OtherDerived>
        CWiseBinaryOp<ScalarSubtractOp<Scalar>, Derived, OtherDerived> operator-(const MatrixBase<OtherDerived>& other) const
        {
            return {Expression(), other.Expression()};
        }

        template<typename OtherDerived>
        CWiseBinaryOp<ScalarDivideOp<Scalar>, Derived, OtherDerived> operator/(const MatrixBase<OtherDerived>& other) const
        {
            return {Expression(), other.Expression()};
        }

        template<typename OtherDerived>
        CWiseBinaryOp<ScalarProductOp<Scalar>, Derived, OtherDerived> CWiseProduct(const MatrixBase<OtherDerived>& other) const
        {
            return {Expression(), other.Expression()};
        }

        template<typename OtherDerived>
        Product<Derived, OtherDerived> operator*(const MatrixBase<OtherDerived>& other) const
        {
            return {Expression(), other.Expression()};
        }

        template<typename BinaryOp>
        Scalar Reduction(BinaryOp functor) const
        {
            return CallReduction(Expression(), functor);
        }

        Scalar Sum() const
        {
            return Reduction(ScalarAddOp<Scalar>{});
        }

        template<std::size_t StartRow, std::size_t StartColumn, std::size_t BlockRows, std::size_t BlockCols>
        Math::Block<Derived, StartRow, StartColumn, BlockRows, BlockCols> Block()
        {
            return Math::Block<Derived, StartRow, StartColumn, BlockRows, BlockCols>(Expression());
        }

        template<std::size_t StartRow, std::size_t StartColumn, std::size_t BlockRows, std::size_t BlockCols>
        Math::Block<Derived, StartRow, StartColumn, BlockRows, BlockCols> Block() const
        {
            return Math::Block<const Derived, StartRow, StartColumn, BlockRows, BlockCols>(Expression());
        }

        Math::Transpose<Derived> Transpose()
        {
            return Math::Transpose<Derived>(Expression());
        }

        Math::Transpose<const Derived> Transpose() const
        {
            return Math::Transpose<const Derived>(Expression());
        }
    };
}
