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
        static constexpr XprFlag        Flags = (Traits<NestedXpr>::Flags & (XprFlag::RowMajor | XprFlag::LeftValue)) |
                (HasFlag<NestedXpr, XprFlag::LinearAccess> ? XprFlag::LinearAccess : XprFlag::None);
    };

    template<typename NestedXpr>
    class Transpose : public MatrixBase<Transpose<NestedXpr>>
    {
    public:
        explicit Transpose(const NestedXpr& xpr) : m_xpr(xpr) {}

        const NestedXpr& NestedExpression() const noexcept { return m_xpr; }

    private:
        const NestedXpr& m_xpr;
    };

    template<typename NestedXpr, bool IsLeftValue = HasFlag<NestedXpr, XprFlag::LeftValue>>
    class TransposeEvaluator;

    template<typename NestedXpr>
    class Evaluator<Transpose<NestedXpr>> : public TransposeEvaluator<NestedXpr>
    {
        using Base = TransposeEvaluator<NestedXpr>;

    public:
        using XprType = Transpose<NestedXpr>;
        using Scalar = Traits<NestedXpr>::Scalar;

        explicit Evaluator(const XprType& transpose) : Base(transpose) {}
    };

    template<typename NestedXpr>
    class TransposeEvaluator<NestedXpr, false>
    {
    public:
        using Xpr = Transpose<NestedXpr>;
        using Scalar = Traits<Xpr>::Scalar;

        explicit TransposeEvaluator(const Xpr& transpose) : m_xprEvaluator(transpose.NestedExpression()) {}

        [[nodiscard]] Scalar Entry(std::size_t index) const
        {
            return m_xprEvaluator.Entry(index);
        }

        [[nodiscard]] Scalar Entry(std::size_t row, std::size_t column) const
        {
            return m_xprEvaluator.Entry(column, row);
        }

    protected:
        Evaluator<NestedXpr> m_xprEvaluator;
    };

    template<typename NestedXpr>
    class TransposeEvaluator<NestedXpr, true> : public TransposeEvaluator<NestedXpr, false>
    {
        using Base = TransposeEvaluator<NestedXpr, false>;
        using Xpr = Base::Xpr;
        using Scalar = Base::Scalar;
        using Base::m_xprEvaluator;

    public:
        explicit TransposeEvaluator(const Xpr& transpose) : Base(transpose) {}

        Scalar& EntryRef(std::size_t index)
        {
            return m_xprEvaluator.Entry(index);
        }

        Scalar& EntryRef(std::size_t row, std::size_t column)
        {
            return m_xprEvaluator.Entry(column, row);
        }
    };
}
