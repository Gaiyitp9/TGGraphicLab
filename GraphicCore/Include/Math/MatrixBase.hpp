/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Declarations.hpp"
#include "Reduction.hpp"
#include "Determinant.hpp"
#include "OrthoMethod.hpp"

namespace TG::Math
{
    template<typename Derived> requires IsMatrixExpression<Derived>
    class MatrixBase
    {
        using Scalar = Traits<Derived>::Scalar;
        static constexpr bool IsVector = Traits<Derived>::Rows == 1 || Traits<Derived>::Columns == 1;
        static constexpr bool IsSquare = Traits<Derived>::Rows == Traits<Derived>::Columns;

    public:
        const Derived& Expression() const noexcept { return *static_cast<Derived const*>(this); }
        Derived& Expression() noexcept { return *static_cast<Derived*>(this); }

        static constexpr std::size_t Rows() noexcept { return Traits<Derived>::Rows; }
        static constexpr std::size_t Columns() noexcept { return Traits<Derived>::Columns; }

        static CWiseNullaryOp<ScalarIdentityOp<Scalar>, PlainMatrix<Derived>> Identity()
        {
            return CWiseNullaryOp<ScalarIdentityOp<Scalar>, PlainMatrix<Derived>>{{}};
        }

        static CWiseNullaryOp<ScalarConstantOp<Scalar>, PlainMatrix<Derived>> Constant(Scalar scalar)
        {
            return CWiseNullaryOp<ScalarConstantOp<Scalar>, PlainMatrix<Derived>>{
                ScalarConstantOp<Scalar>{scalar}};
        }

        static CWiseNullaryOp<ScalarConstantOp<Scalar>, PlainMatrix<Derived>> Zero()
        {
            return Constant(Scalar{0});
        }

        template<typename OtherDerived>
        CWiseBinaryOp<ScalarAddOp<Scalar>, Derived, OtherDerived>
            operator+(const MatrixBase<OtherDerived>& other) const
        {
            return {Expression(), other.Expression()};
        }

        template<typename OtherDerived>
        CWiseBinaryOp<ScalarSubtractOp<Scalar>, Derived, OtherDerived>
            operator-(const MatrixBase<OtherDerived>& other) const
        {
            return {Expression(), other.Expression()};
        }

        template<typename OtherDerived>
        CWiseBinaryOp<ScalarDivideOp<Scalar>, Derived, OtherDerived>
            operator/(const MatrixBase<OtherDerived>& other) const
        {
            return {Expression(), other.Expression()};
        }

        template<typename OtherDerived>
        Product<Derived, OtherDerived, ProductType::Default>
            operator*(const MatrixBase<OtherDerived>& other) const
        {
            return {Expression(), other.Expression()};
        }
        // 矩阵乘常量，常量乘矩阵需要单独实现operator*
        Product<Derived, CWiseNullaryOp<ScalarConstantOp<Scalar>, PlainMatrix<Derived>>, ProductType::Default>
            operator*(Scalar scalar) const
        {
            return {Expression(), Constant(scalar)};
        }

        template<typename OtherDerived>
        CWiseBinaryOp<ScalarProductOp<Scalar>, Derived, OtherDerived>
            CWiseProduct(const MatrixBase<OtherDerived>& other) const
        {
            return {Expression(), other.Expression()};
        }

        template<typename OtherDerived>
        Product<Derived, OtherDerived, ProductType::Lazy> LazyProduct(const MatrixBase<OtherDerived>& other) const
        {
            return {Expression(), other.Expression()};
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

        template<typename OtherDerived> requires IsVector
        [[nodiscard]] Scalar Dot(const MatrixBase<OtherDerived>& other) const
        {
            return CWiseProduct(other).Sum();
        }

        template<typename OtherDerived> requires IsVector
        [[nodiscard]] CrossImpl<Derived, OtherDerived>::ReturnType Cross(const MatrixBase<OtherDerived>& other) const
        {
            return CrossImpl<Derived, OtherDerived>{}(Expression(), other.Expression());
        }

        template<std::size_t BlockRows, std::size_t BlockCols>
        Block<Derived, BlockRows, BlockCols> block(std::size_t startRow, std::size_t startColumn)
        {
            return {Expression(), startRow, startColumn};
        }

        template<std::size_t BlockRows, std::size_t BlockCols>
        Block<const Derived, BlockRows, BlockCols> block(std::size_t startRow, std::size_t startColumn) const
        {
            return {Expression(), startRow, startColumn};
        }

        Transpose<Derived> transpose() const
        {
            return Transpose<Derived>(Expression());
        }

        Block<Derived, 1, Traits<Derived>::Columns> Row(std::size_t row)
        {
            return {Expression(), row, 0};
        }

        Block<const Derived, 1, Traits<Derived>::Columns> Row(std::size_t row) const
        {
            return {Expression(), row, 0};
        }

        Block<Derived, Traits<Derived>::Rows, 1> Column(std::size_t column)
        {
            return {Expression(), 0, column};
        }

        Block<const Derived, Traits<Derived>::Rows, 1> Column(std::size_t column) const
        {
            return {Expression(), 0, column};
        }

        Scalar Determinant() const requires IsSquare
        {
            return DeterminantImpl<Derived, Traits<Derived>::Rows>{}(Expression());
        }

        Inverse<Derived> inverse() const requires IsSquare
        {
            return Inverse<Derived>(Expression());
        }
    };
}
