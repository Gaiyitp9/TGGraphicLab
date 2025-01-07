/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "MatrixBase.hpp"

namespace TG::Math
{
    template<typename NullaryOp, typename Matrix>
    struct Traits<CWiseNullaryOp<NullaryOp, Matrix>>
    {
        using Scalar = Traits<Matrix>::Scalar;
        static constexpr std::size_t	Rows = Traits<Matrix>::Rows;
        static constexpr std::size_t	Columns = Traits<Matrix>::Columns;
        static constexpr std::size_t	Size = Rows * Columns;
        static constexpr XprFlag        Flags = Traits<Matrix>::Flags & XprFlag::RowMajor | XprFlag::LinearAccess;
    };

    template<typename NullaryOp, typename Matrix>
    class CWiseNullaryOp : public MatrixBase<CWiseNullaryOp<NullaryOp, Matrix>>
    {
    public:
        explicit CWiseNullaryOp(NullaryOp nullaryOp) : m_nullaryOp(nullaryOp) {}

        NullaryOp Functor() const { return m_nullaryOp; }

    private:
        NullaryOp m_nullaryOp;
    };

    template<typename NullaryOp, typename Matrix>
    class Evaluator<CWiseNullaryOp<NullaryOp, Matrix>>
    {
        using Xpr = CWiseNullaryOp<NullaryOp, Matrix>;
        using Scalar = Traits<Xpr>::Scalar;

    public:
        explicit Evaluator(const Xpr& xpr) : m_nullaryOp(xpr.Functor()) {}

        Scalar Entry(std::size_t index) const
        {
            std::size_t row, column;
            if constexpr (HasFlag<Xpr, XprFlag::RowMajor>)
            {
                row = index / Traits<Xpr>::Columns;
                column = index % Traits<Xpr>::Columns;
            }
            else
            {
                row = index % Traits<Xpr>::Rows;
                column = index / Traits<Xpr>::Rows;
            }
            return Entry(row, column);
        }

        Scalar Entry(std::size_t row, std::size_t column) const
        {
            return m_nullaryOp(row, column);
        }

    private:
        NullaryOp m_nullaryOp;
    };

    template<typename Scalar>
    struct ScalarIdentityOp
    {
        Scalar operator()(std::size_t row, std::size_t column) const
        {
            return row == column ? Scalar(1) : Scalar(0);
        }
    };

    template<typename Scalar>
    struct ScalarConstantOp
    {
        explicit ScalarConstantOp(Scalar constant) : m_constant(constant) {}

        Scalar operator()(std::size_t row, std::size_t column) const
        {
            return m_constant;
        }

    private:
        Scalar m_constant;
    };
}
