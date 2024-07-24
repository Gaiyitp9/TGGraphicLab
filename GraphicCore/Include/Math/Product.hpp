/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename LhsXpr, typename RhsXpr>
    struct Traits<Product<LhsXpr, RhsXpr>>
    {
        using Scalar = Traits<LhsXpr>::Scalar;
        static constexpr int	    Rows = Traits<LhsXpr>::Rows;
        static constexpr int	    Columns = Traits<RhsXpr>::Columns;
        static constexpr int	    Size = Rows * Columns;
        static constexpr XprFlag    Flags = (Traits<LhsXpr>::Flags == StorageOrder::RowMajor ? XprFlag::RowMajor : XprFlag::None);
    };

    // 矩阵乘法表达式
	template<typename LhsXpr, typename RhsXpr> requires MatrixMultipliable<LhsXpr, RhsXpr>
	class Product : public MatrixBase<Product<LhsXpr, RhsXpr>>
	{
    public:
        Product(const LhsXpr& lhs, const RhsXpr& rhs) : m_lhs(lhs), m_rhs(rhs) {}

        const LhsXpr& LhsExpression() const { return m_lhs; }
        const RhsXpr& RhsExpression() const { return m_rhs; }

    private:
        const LhsXpr& m_lhs;
        const RhsXpr& m_rhs;
	};

    // 矩阵乘法求值器
    template<typename LhsXpr, typename RhsXpr>
    class Evaluator<Product<LhsXpr, RhsXpr>>
    {
        using Lhs = PlainMatrixType<LhsXpr>::Type;
        using Rhs = PlainMatrixType<RhsXpr>::Type;

    public:
        using XprType = Product<LhsXpr, RhsXpr>;
        using Scalar = Traits<XprType>::Scalar;

        explicit Evaluator(const XprType& xpr) : m_lhsEvaluator(xpr.LhsExpression()),
            m_rhsEvaluator(xpr.RhsExpression()) {}

        Scalar Coefficient(int index) const
        {
            int row = index / Traits<XprType>::Columns;
            int col = index % Traits<XprType>::Columns;
            return Coefficient(row, col);
        }

        Scalar Coefficient(int row, int col) const
        {
            Scalar coefficient = 0;
            for (int i = 0; i < Traits<XprType>::Columns; ++i)
                coefficient += m_lhsEvaluator.Coefficient(row, i) * m_rhsEvaluator.Coefficient(i, col);
            return coefficient;
        }

    private:
        Evaluator<Lhs> m_lhsEvaluator;
        Evaluator<Rhs> m_rhsEvaluator;
    };
}
