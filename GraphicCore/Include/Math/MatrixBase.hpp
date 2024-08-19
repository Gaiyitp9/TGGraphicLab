/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Declarations.hpp"
#include "Assignment.hpp"
#include "Reduction.hpp"

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
        [[nodiscard]] Scalar Reduce(BinaryOp functor) const
        {
            Reduction reduction{Expression(), functor};
            return reduction.Run();
        }

        [[nodiscard]] Scalar Sum() const
        {
            return Reduce(ScalarAddOp<Scalar>{});
        }

        template<typename OtherDerived> requires HasFlag<Derived, XprFlag::IsVector>
        [[nodiscard]] Scalar Dot(const MatrixBase<OtherDerived>& other) const
        {
            return CWiseProduct(other).Sum();
        }

        template<std::size_t StartRow, std::size_t StartColumn, std::size_t BlockRows, std::size_t BlockCols>
        Block<Derived, StartRow, StartColumn, BlockRows, BlockCols> block()
        {
            return Block<Derived, StartRow, StartColumn, BlockRows, BlockCols>(Expression());
        }

        template<std::size_t StartRow, std::size_t StartColumn, std::size_t BlockRows, std::size_t BlockCols>
        Block<const Derived, StartRow, StartColumn, BlockRows, BlockCols> block() const
        {
            return Block<const Derived, StartRow, StartColumn, BlockRows, BlockCols>(Expression());
        }

        Transpose<Derived> transpose()
        {
            return Transpose<Derived>(Expression());
        }

        Transpose<const Derived> transpose() const
        {
            return Transpose<const Derived>(Expression());
        }
    };
}
