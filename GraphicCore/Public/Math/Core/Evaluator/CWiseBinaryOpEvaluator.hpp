/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename BinaryOp, typename LhsXpr, typename RhsXpr>
    class Evaluator<CWiseBinaryOp<BinaryOp, LhsXpr, RhsXpr>>
    {
        using Xpr = CWiseBinaryOp<BinaryOp, LhsXpr, RhsXpr>;
        using Scalar = Traits<Xpr>::Scalar;

    public:
        explicit Evaluator(const Xpr& xpr) : m_functor(xpr.Functor()),
            m_lhsEvaluator(xpr.LhsExpression()), m_rhsEvaluator(xpr.RhsExpression()) {}

        [[nodiscard]] Scalar Entry(std::size_t index) const requires HasFlag<Xpr, XprFlag::LinearAccess>
        {
            return m_functor(m_lhsEvaluator.Entry(index), m_rhsEvaluator.Entry(index));
        }
        [[nodiscard]] Scalar Entry(std::size_t row, std::size_t col) const
        {
            return m_functor(m_lhsEvaluator.Entry(row, col), m_rhsEvaluator.Entry(row, col));
        }

    private:
        const BinaryOp m_functor;
        Evaluator<LhsXpr> m_lhsEvaluator;
        Evaluator<RhsXpr> m_rhsEvaluator;
    };
}
