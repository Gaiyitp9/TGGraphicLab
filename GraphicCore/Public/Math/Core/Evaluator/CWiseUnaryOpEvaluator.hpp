/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename UnaryOp, typename ArgXpr>
    class Evaluator<CWiseUnaryOp<UnaryOp, ArgXpr>>
    {
        using Xpr = CWiseUnaryOp<UnaryOp, ArgXpr>;
        using Scalar = Traits<Xpr>::Scalar;

    public:
        explicit Evaluator(const Xpr& xpr)
            : m_unaryOp(xpr.Functor()), m_argXprEvaluator(xpr.NestedExpression())
        {}

        Scalar Entry(std::size_t index) const requires HasFlag<Xpr, XprFlag::LinearAccess>
        {
            return m_unaryOp(m_argXprEvaluator.Entry(index));
        }

        Scalar Entry(std::size_t row, std::size_t column) const
        {
            return m_unaryOp(m_argXprEvaluator.Entry(row, column));
        }

    private:
        const UnaryOp m_unaryOp;
        Evaluator<ArgXpr> m_argXprEvaluator;
    };
}
