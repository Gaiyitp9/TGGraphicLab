/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "MatrixBase.hpp"

namespace TG::Math
{
    template<typename LhsXpr, typename RhsXpr>
    struct Traits<Product<LhsXpr, RhsXpr>>
    {
        using Scalar = Traits<LhsXpr>::Scalar;
        static constexpr std::size_t    Rows = Traits<LhsXpr>::Rows;
        static constexpr std::size_t	Columns = Traits<RhsXpr>::Columns;
        static constexpr std::size_t	Size = Rows * Columns;
        static constexpr XprFlag        Flags = (Traits<LhsXpr>::Flags == StorageOrder::RowMajor ?
            XprFlag::RowMajor : XprFlag::None) | XprFlag::LinearAccess;
    };

    // 矩阵乘法表达式
	template<typename LhsXpr, typename RhsXpr>
	class Product final : public MatrixBase<Product<LhsXpr, RhsXpr>>
	{
    public:
        Product(const LhsXpr& lhs, const RhsXpr& rhs) : m_lhs(lhs), m_rhs(rhs) {}

        const LhsXpr& LhsExpression() const { return m_lhs; }
        const RhsXpr& RhsExpression() const { return m_rhs; }

    private:
        const LhsXpr& m_lhs;
        const RhsXpr& m_rhs;
	};

    // 两个表达式是否可以执行矩阵乘法，左边表达式的列数要等于右边表达式的行数
    template<typename LhsXpr, typename RhsXpr>
    concept MatrixMultipliable = std::is_same_v<typename Traits<LhsXpr>::Scalar, typename Traits<RhsXpr>::Scalar> &&
            Traits<LhsXpr>::Columns == Traits<RhsXpr>::Rows;

    // 矩阵乘法求值器
    template<typename LhsXpr, typename RhsXpr>  requires MatrixMultipliable<LhsXpr, RhsXpr>
    class Evaluator<Product<LhsXpr, RhsXpr>>
    {
        using Xpr = Product<LhsXpr, RhsXpr>;
        using Scalar = Traits<Xpr>::Scalar;

    public:
        explicit Evaluator(const Xpr& xpr) : m_lhsEvaluator(xpr.LhsExpression()),
            m_rhsEvaluator(xpr.RhsExpression()) {}

        Scalar Entry(std::size_t index) const
        {
            int row = index / Traits<Xpr>::Columns;
            int col = index % Traits<Xpr>::Columns;
            return Entry(row, col);
        }

        Scalar Entry(std::size_t row, std::size_t col) const
        {
            Scalar coefficient = 0;
            for (int i = 0; i < Traits<Xpr>::Columns; ++i)
                coefficient += m_lhsEvaluator.Entry(row, i) * m_rhsEvaluator.Entry(i, col);
            return coefficient;
        }

    private:
        Evaluator<LhsXpr> m_lhsEvaluator;
        Evaluator<RhsXpr> m_rhsEvaluator;
        Matrix<typename Traits<Xpr>::Scalar, Traits<Xpr>::Rows, Traits<Xpr>::Columns,
                HasFlag<Xpr, XprFlag::RowMajor> ? StorageOrder::RowMajor : StorageOrder::ColumnMajor> m_product;
    };
}
