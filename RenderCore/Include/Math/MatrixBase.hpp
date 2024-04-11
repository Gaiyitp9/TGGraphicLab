/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename Derived>
    class MatrixBase
    {
        using Scalar = Traits<Derived>::Scalar;

    public:
        const Derived& Expression() const { return *static_cast<const Derived*>(this); }
        Derived& Expression() { return *static_cast<Derived*>(this); }

        template<typename OtherDerived>
        CWiseBinaryOp<ScalarSumOp<Scalar>, Derived, OtherDerived> operator+(const MatrixBase<OtherDerived>& other) const
        {
            return { Expression(), other.Expression() };
        }

        template<typename OtherDerived>
        CWiseBinaryOp<ScalarSubtractOp<Scalar>, Derived, OtherDerived> operator-(const MatrixBase<OtherDerived>& other) const
        {
            return { Expression(), other.Expression() };
        }

        template<typename OtherDerived>
        CWiseBinaryOp<ScalarDivideOp<Scalar>, Derived, OtherDerived> operator/(const MatrixBase<OtherDerived>& other) const
        {
            return { Expression(), other.Expression() };
        }

        template<typename OtherDerived>
        CWiseBinaryOp<ScalarProductOp<Scalar>, Derived, OtherDerived> CWiseProduct(const MatrixBase<OtherDerived>& other) const
        {
            return { Expression(), other.Expression() };
        }

        template<typename OtherDerived>
        Product<Derived, OtherDerived> operator*(const MatrixBase<OtherDerived>& other) const
        {
            return { Expression(), other.Expression() };
        }

        template<typename BinaryOp>
        Scalar Redux(BinaryOp functor) const
        {
            return CallRedux(Expression(), functor);
        }

        Scalar Sum() const
        {
            return Redux(std::plus<Scalar>{});
        }
    };
}
