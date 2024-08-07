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
        static constexpr XprFlag        Flags = Traits<NestedXpr>::Flags & ~XprFlag::LinearAccess;
    };

    template<typename NestedXpr>
    class Transpose : public MatrixBase<Transpose<NestedXpr>>
    {
    public:
        explicit Transpose(NestedXpr& xpr) : m_xpr(xpr) {}

        const NestedXpr& NestedExpression() const noexcept { return m_xpr; }
        NestedXpr& NestedExpression() noexcept { return m_xpr; }

    private:
        NestedXpr& m_xpr;
    };

    template<typename NestedXpr, bool IsLeftValue = HasFlag<NestedXpr, XprFlag::LeftValue>>
    class TransposeEvaluator;

    template<typename NestedXpr, bool IsConst>
    class Evaluator<Transpose<NestedXpr>, IsConst>
    {
        using Xpr = Transpose<NestedXpr>;
        using InternalXpr = std::conditional_t<IsConst, const Xpr, Xpr>;
        using Scalar = Traits<Xpr>::Scalar;

    public:
        explicit Evaluator(InternalXpr& transpose) : m_xprEvaluator(transpose.NestedExpression()) {}

        [[nodiscard]] Scalar Entry(std::size_t index) const requires HasFlag<Xpr, XprFlag::LinearAccess>
        {
            return m_xprEvaluator.Entry(index);
        }

        [[nodiscard]] Scalar Entry(std::size_t row, std::size_t column) const
        {
            return m_xprEvaluator.Entry(column, row);
        }

        Scalar& Entry(std::size_t index) requires (HasFlag<Xpr, XprFlag::LinearAccess> && !IsConst)
        {
            return m_xprEvaluator.Entry(index);
        }

        Scalar& Entry(std::size_t row, std::size_t column) requires !IsConst
        {
            return m_xprEvaluator.Entry(column, row);
        }

    private:
        Evaluator<NestedXpr, IsConst> m_xprEvaluator;
    };
}
