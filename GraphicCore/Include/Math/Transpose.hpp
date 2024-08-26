/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename NestedXpr>
    struct Traits<Transpose<NestedXpr>>
    {
        using Scalar = Traits<NestedXpr>::Scalar;
        static constexpr std::size_t    Rows = Traits<NestedXpr>::Columns;
        static constexpr std::size_t    Columns = Traits<NestedXpr>::Rows;
        static constexpr std::size_t    Size = Rows * Columns;
        // 存储顺序翻转，移除左值标志，其他的标志不变
        static constexpr XprFlag        Flags = (Traits<NestedXpr>::Flags ^ XprFlag::RowMajor) & ~XprFlag::LeftValue;
    };

    template<typename NestedXpr>
    class Transpose final : public MatrixBase<Transpose<NestedXpr>>
    {
    public:
        explicit Transpose(const NestedXpr& xpr) : m_xpr(xpr) {}

        [[nodiscard]] const NestedXpr& NestedExpression() const noexcept { return m_xpr; }

    private:
        const NestedXpr& m_xpr;
    };

    template<typename NestedXpr>
    class Evaluator<Transpose<NestedXpr>>
    {
        using Xpr = Transpose<NestedXpr>;
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
        Evaluator<NestedXpr> m_xprEvaluator;
    };
}
