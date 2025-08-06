/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename ArgXpr>
    class Evaluator<Transpose<ArgXpr>>
    {
        using Xpr = Transpose<ArgXpr>;
        using Scalar = Traits<Xpr>::Scalar;

    public:
        explicit Evaluator(const Xpr& transpose) : m_xprEvaluator(transpose.NestedExpression()) {}

        [[nodiscard]] Scalar Entry(std::size_t index) const requires HasFlag<Xpr, XprFlag::LinearAccess>
        {
            return m_xprEvaluator.Entry(index);
        }

        [[nodiscard]] Scalar Entry(std::size_t row, std::size_t column) const
        {
            return m_xprEvaluator.Entry(column, row);
        }

    private:
        Evaluator<ArgXpr> m_xprEvaluator;
    };
}
