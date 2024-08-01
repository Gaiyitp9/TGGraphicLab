/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename BinaryOp, typename LhsXpr, typename RhsXpr>
    struct Traits<CWiseBinaryOp<BinaryOp, LhsXpr, RhsXpr>>
    {
        using Scalar = Traits<LhsXpr>::Scalar;
        static constexpr std::size_t	Rows = Traits<LhsXpr>::Rows;
        static constexpr std::size_t	Columns = Traits<LhsXpr>::Columns;
        static constexpr std::size_t	Size = Rows * Columns;
        static constexpr XprFlag        Flags = Traits<LhsXpr>::Flags & (~XprFlag::LeftValue);
    };

    // 二元表达式
	template<typename BinaryOp, typename LhsXpr, typename RhsXpr> requires CWiseOperable<LhsXpr, RhsXpr>
	class CWiseBinaryOp final : public MatrixBase<CWiseBinaryOp<BinaryOp, LhsXpr, RhsXpr>>
	{
    public:
        CWiseBinaryOp(LhsXpr& lhs, RhsXpr& rhs, BinaryOp op = {})
            : m_functor(op), m_lhs(lhs), m_rhs(rhs) {}
	    ~CWiseBinaryOp() override = default;

        [[nodiscard]] BinaryOp Functor() const noexcept { return m_functor; }
        [[nodiscard]] const LhsXpr& LhsExpression() const noexcept { return m_lhs; }
        [[nodiscard]] const RhsXpr& RhsExpression() const noexcept { return m_rhs; }
	    LhsXpr& LhsExpression() noexcept { return m_lhs; }
	    RhsXpr& RhsExpression() noexcept { return m_rhs; }

    private:
        const BinaryOp m_functor;
        LhsXpr& m_lhs;
        RhsXpr& m_rhs;
	};

    template<typename BinaryOp, typename LhsXpr, typename RhsXpr>
    class Evaluator<CWiseBinaryOp<BinaryOp, LhsXpr, RhsXpr>>
    {
    public:
        using Xpr = CWiseBinaryOp<BinaryOp, LhsXpr, RhsXpr>;

        explicit Evaluator(Xpr& xpr) : m_functor(xpr.Functor()),
            m_lhsEvaluator(xpr.LhsExpression()), m_rhsEvaluator(xpr.RhsExpression()) {}

        [[nodiscard]] Traits<Xpr>::Scalar Entry(std::size_t index) const
        {
            return m_functor(m_lhsEvaluator.Entry(index), m_rhsEvaluator.Entry(index));
        }
        [[nodiscard]] Traits<Xpr>::Scalar Entry(std::size_t row, std::size_t col) const
        {
            return m_functor(m_lhsEvaluator.Entry(row), m_rhsEvaluator.Entry(col));
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
