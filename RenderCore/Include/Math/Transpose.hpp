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
                (CheckFlag<NestedXpr>(XprFlag::LinearAccess) && CheckFlag<NestedXpr>(XprFlag::Vector) ?
                XprFlag::LinearAccess : XprFlag::None) |
                (Rows == 1 || Columns == 1 ? XprFlag::Vector : XprFlag::None) |
                (Rows == Columns ? XprFlag::Square : XprFlag::None);
    };

    template<typename NestedXpr>
    class Transpose : public MatrixBase<Transpose<NestedXpr>>
    {
    public:
        explicit Transpose(const NestedXpr& xpr) : m_xpr(xpr) {}

        const NestedXpr& NestedExpression() const { return m_xpr; }

    private:
        const NestedXpr& m_xpr;
    };

    template<typename NestedXpr, bool IsLeftValue = CheckFlag<NestedXpr>(XprFlag::LeftValue)>
    class TransposeEvaluator;

    template<typename NestedXpr>
    class Evaluator<Transpose<NestedXpr>> : public TransposeEvaluator<NestedXpr>
    {
        using Base = TransposeEvaluator<NestedXpr>;

    public:
        using XprType = Transpose<NestedXpr>;
        using CoeffType = Traits<NestedXpr>::Scalar;

        explicit Evaluator(const XprType& transpose) : Base(transpose) {}
    };

    template<typename NestedXpr>
    class TransposeEvaluator<NestedXpr, false>
    {
    public:
        using XprType = Transpose<NestedXpr>;
        using CoeffType = Traits<XprType>::Scalar;

        explicit TransposeEvaluator(const XprType& transpose) : m_xprEvaluator(transpose.NestedExpression()) {}

        [[nodiscard]] CoeffType Coefficient(std::size_t index) const
        {
            return m_xprEvaluator.Coefficient(index);
        }

        [[nodiscard]] CoeffType Coefficient(std::size_t row, std::size_t column) const
        {
            return m_xprEvaluator.Coefficient(column, row);
        }

    protected:
        Evaluator<NestedXpr> m_xprEvaluator;
    };

    template<typename NestedXpr>
    class TransposeEvaluator<NestedXpr, true> : public TransposeEvaluator<NestedXpr, false>
    {
        using Base = TransposeEvaluator<NestedXpr, false>;
        using XprType = Base::XprType;
        using CoeffType = Base::CoeffType;
        using Base::m_xprEvaluator;

    public:
        explicit TransposeEvaluator(const XprType& transpose) : Base(transpose) {}

        CoeffType& CoefficientRef(std::size_t index)
        {
            return m_xprEvaluator.Coefficient(index);
        }

        CoeffType& CoefficientRef(std::size_t row, std::size_t column)
        {
            return m_xprEvaluator.Coefficient(column, row);
        }
    };
}
