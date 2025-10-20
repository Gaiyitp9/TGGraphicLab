/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
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
}
