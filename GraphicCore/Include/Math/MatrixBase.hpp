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

        const Derived& Expression() const { return *static_cast<const Derived*>(this); }
        Derived& Expression() { return *static_cast<Derived*>(this); }

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
        Scalar Redux(BinaryOp functor) const
        {
            return CallRedux(Expression(), functor);
        }

        Scalar Sum() const
        {
            return Redux(ScalarAddOp<Scalar>{});
        }

        template<std::size_t StartRow, std::size_t StartColumn, std::size_t BlockRows, std::size_t BlockCols>
        Block<Derived, StartRow, StartColumn, BlockRows, BlockCols> Block()
        {
            return Math::Block<Derived, StartRow, StartColumn, BlockRows, BlockCols>(Expression());
        }

        Transpose<Derived> Transpose()
        {
            return Math::Transpose<Derived>(Expression());
        }
    };
}
