/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "MatrixBase.hpp"

namespace TG::Math
{
    template<typename BinaryOp, typename LhsXpr, typename RhsXpr>
    struct Traits<CWiseBinaryOp<BinaryOp, LhsXpr, RhsXpr>>
    {
        using Scalar = Traits<LhsXpr>::Scalar;
        static constexpr std::size_t	Rows = Traits<LhsXpr>::Rows;
        static constexpr std::size_t	Columns = Traits<LhsXpr>::Columns;
        static constexpr std::size_t	Size = Rows * Columns;
        // 非左值；如果两个表达式存储顺序不同，则不能线性访问
        static constexpr XprFlag        Flags = Traits<LhsXpr>::Flags & Traits<RhsXpr>::Flags &
            ~XprFlag::LeftValue &
            (HasFlag<LhsXpr, XprFlag::RowMajor> == HasFlag<RhsXpr, XprFlag::RowMajor> ?
                ~XprFlag::None : ~XprFlag::LinearAccess);
    };

    // 二元表达式
	template<typename BinaryOp, typename LhsXpr, typename RhsXpr> requires CWiseOperable<LhsXpr, RhsXpr>
	class CWiseBinaryOp final : public MatrixBase<CWiseBinaryOp<BinaryOp, LhsXpr, RhsXpr>>
	{
    public:
        CWiseBinaryOp(const LhsXpr& lhs, const RhsXpr& rhs, BinaryOp op = {})
            : m_functor(op), m_lhs(lhs), m_rhs(rhs) {}

        [[nodiscard]] BinaryOp Functor() const noexcept { return m_functor; }
        [[nodiscard]] const LhsXpr& LhsExpression() const noexcept { return m_lhs; }
        [[nodiscard]] const RhsXpr& RhsExpression() const noexcept { return m_rhs; }

    private:
        const BinaryOp m_functor;
        const LhsXpr& m_lhs;
        const RhsXpr& m_rhs;
	};

    template<typename BinaryOp, typename LhsXpr, typename RhsXpr>
    class Evaluator<CWiseBinaryOp<BinaryOp, LhsXpr, RhsXpr>, true>
    {
    public:
        using Xpr = CWiseBinaryOp<BinaryOp, LhsXpr, RhsXpr>;

        explicit Evaluator(const Xpr& xpr) : m_functor(xpr.Functor()),
            m_lhsEvaluator(xpr.LhsExpression()), m_rhsEvaluator(xpr.RhsExpression()) {}

        [[nodiscard]] Traits<Xpr>::Scalar Entry(std::size_t index) const requires HasFlag<Xpr, XprFlag::LinearAccess>
        {
            return m_functor(m_lhsEvaluator.Entry(index), m_rhsEvaluator.Entry(index));
        }
        [[nodiscard]] Traits<Xpr>::Scalar Entry(std::size_t row, std::size_t col) const
        {
            return m_functor(m_lhsEvaluator.Entry(row, col), m_rhsEvaluator.Entry(row, col));
        }

    private:
        const BinaryOp m_functor;
        ConstEvaluator<LhsXpr> m_lhsEvaluator;
        ConstEvaluator<RhsXpr> m_rhsEvaluator;
    };

    template<typename Scalar>
    struct ScalarAddOp
    {
        [[nodiscard]] Scalar operator()(Scalar a, Scalar b) const { return a + b; }
    };
    template<typename Scalar>
    struct ScalarSubtractOp
    {
        [[nodiscard]] Scalar operator()(Scalar a, Scalar b) const { return a - b; }
    };
    template<typename Scalar>
    struct ScalarProductOp
    {
        [[nodiscard]] Scalar operator()(Scalar a, Scalar b) const { return a * b; }
    };
    template<typename Scalar>
    struct ScalarDivideOp
    {
        [[nodiscard]] Scalar operator()(Scalar a, Scalar b) const { return a / b; }
    };
}
