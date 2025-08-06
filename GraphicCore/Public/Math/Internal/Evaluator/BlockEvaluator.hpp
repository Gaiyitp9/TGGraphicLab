/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename ArgXpr, std::size_t BlockRows, std::size_t BlockColumns, bool IsConst>
    class Evaluator<Block<ArgXpr, BlockRows, BlockColumns>, IsConst>
    {
        using Xpr = Block<ArgXpr, BlockRows, BlockColumns>;
        using InternalXpr = std::conditional_t<IsConst, const Xpr, Xpr>;
        using Scalar = Traits<Xpr>::Scalar;

    public:
        explicit Evaluator(InternalXpr& block) : m_xprEvaluator(block.NestedExpression()),
            m_startRow(block.StartRow()), m_startColumn(block.StartColumn()),
            m_offset(HasFlag<Xpr, XprFlag::RowMajor> ? m_startRow * Traits<ArgXpr>::Columns + m_startColumn :
                m_startColumn * Traits<ArgXpr>::Rows + m_startRow)
        {}

        [[nodiscard]] Scalar Entry(std::size_t index) const requires HasFlag<Xpr, XprFlag::LinearAccess>
        {
            return m_xprEvaluator.Entry(m_offset + index);
        }

        [[nodiscard]] Scalar Entry(std::size_t row, std::size_t column) const
        {
            return m_xprEvaluator.Entry(m_startRow + row, m_startColumn + column);
        }

        Scalar& Entry(std::size_t index) requires (HasFlag<Xpr, XprFlag::LinearAccess> && !IsConst)
        {
            return m_xprEvaluator.Entry(m_offset + index);
        }

        Scalar& Entry(std::size_t row, std::size_t column) requires (!IsConst)
        {
            return m_xprEvaluator.Entry(m_startRow + row, m_startColumn + column);
        }

    private:
        Evaluator<std::remove_const_t<ArgXpr>, IsConst> m_xprEvaluator;
        std::size_t m_startRow;
        std::size_t m_startColumn;
        std::size_t m_offset;
    };
}
