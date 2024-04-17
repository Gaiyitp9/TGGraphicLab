/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename BinaryOp, typename LhsXpr, typename RhsXpr>
    struct Traits<CWiseBinaryOp<BinaryOp, LhsXpr, RhsXpr>> : public Traits<LhsXpr> {};

    // 二元表达式
	template<typename BinaryOp, typename LhsXpr, typename RhsXpr> requires CWiseOperable<LhsXpr, RhsXpr>
	class CWiseBinaryOp : public MatrixBase<CWiseBinaryOp<BinaryOp, LhsXpr, RhsXpr>>
	{
    public:
        CWiseBinaryOp(const LhsXpr& lhs, const RhsXpr& rhs, BinaryOp op = {})
            : m_lhs(lhs), m_rhs(rhs), m_functor(op) {}

        [[nodiscard]] BinaryOp Functor() const { return m_functor; }
        [[nodiscard]] const LhsXpr& LhsExpression() const { return m_lhs; }
        [[nodiscard]] const RhsXpr& RhsExpression() const { return m_rhs; }

    private:
        const BinaryOp m_functor;
        const LhsXpr& m_lhs;
        const RhsXpr& m_rhs;
	};

    // 二元表达式求值器
    template<typename BinaryOp, typename LhsXpr, typename RhsXpr>
    class Evaluator<CWiseBinaryOp<BinaryOp, LhsXpr, RhsXpr>>
    {
    public:
        using XprType = CWiseBinaryOp<BinaryOp, LhsXpr, RhsXpr>;
        using CoeffType = Traits<XprType>::Scalar;

        explicit Evaluator(const XprType& xpr) : m_functor(xpr.Functor()),
            m_lhsEvaluator(xpr.LhsExpression()), m_rhsEvaluator(xpr.RhsExpression()) {}

        [[nodiscard]] CoeffType Coefficient(int index) const
        {
            return m_functor(m_lhsEvaluator.Coefficient(index), m_rhsEvaluator.Coefficient(index));
        }
        [[nodiscard]] CoeffType Coefficient(int row, int col) const
        {
            return m_functor(m_lhsEvaluator.Coefficient(row), m_rhsEvaluator.Coefficient(col));
        }

    private:
        const BinaryOp m_functor;
        Evaluator<LhsXpr> m_lhsEvaluator;
        Evaluator<RhsXpr> m_rhsEvaluator;
    };

    template<typename Scalar>
    struct ScalarSumOp
    {
        Scalar operator()(const Scalar& a, const Scalar& b) const { return a + b; }
    };
    template<typename Scalar>
    struct ScalarSubtractOp
    {
        Scalar operator()(const Scalar& a, const Scalar& b) const { return a - b; }
    };
    template<typename Scalar>
    struct ScalarProductOp
    {
        Scalar operator()(const Scalar& a, const Scalar& b) const { return a * b; }
    };
    template<typename Scalar>
    struct ScalarDivideOp
    {
        Scalar operator()(const Scalar& a, const Scalar& b) const { return a / b; }
    };
}
