/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "MatrixBase.hpp"
#include "Assignment.hpp"

namespace TG::Math
{
    template<typename NestedXpr, std::size_t BlockRows, std::size_t BlockColumns>
    struct Traits<Block<NestedXpr, BlockRows, BlockColumns>>
    {
    private:
        // 因为Block有const和非const两种版本，所以这里要去掉const修饰符来获取无修饰的类型
        using PureXpr = std::remove_const_t<NestedXpr>;
        // Block可以线性访问的条件：
        // 1. 内嵌表达式可以线性访问
        // 2. 对于row major的表达式，Block的列数等于内嵌表达式的列数或者Block行数等于1;
        // 对于column major的表达式，Block的行数等于内嵌表达式的行数或者Block列数等于1
        static constexpr bool LinearAccessible = HasFlag<PureXpr, XprFlag::LinearAccess> &&
            (HasFlag<PureXpr, XprFlag::RowMajor> ? BlockColumns == Traits<PureXpr>::Columns || BlockRows == 1 :
                BlockRows == Traits<PureXpr>::Rows || BlockColumns == 1);

    public:
        using Scalar = Traits<PureXpr>::Scalar;
        static constexpr std::size_t    Rows = BlockRows;
        static constexpr std::size_t    Columns = BlockColumns;
        static constexpr std::size_t    Size = Rows * Columns;
        static constexpr XprFlag        Flags = Traits<PureXpr>::Flags &
            (LinearAccessible ? ~XprFlag::None : ~XprFlag::LinearAccess);
    };

    template<typename NestedXpr, std::size_t BlockRows, std::size_t BlockColumns>
    class Block final : public MatrixBase<Block<NestedXpr, BlockRows, BlockColumns>>
    {
        using PureXpr = std::remove_const_t<NestedXpr>;

    public:
        Block(NestedXpr& xpr, std::size_t startRow, std::size_t startColumn)
            : m_xpr(xpr), m_startRow(startRow), m_startColumn(startColumn) {}

        template<typename Derived>
        Block& operator=(const MatrixBase<Derived>& other)
        {
            CallAssignment(this->Expression(), other.Expression());
            return *this;
        }

        PureXpr& NestedExpression() noexcept { return m_xpr; }
        [[nodiscard]] const PureXpr& NestedExpression() const noexcept { return m_xpr; }

        [[nodiscard]] std::size_t StartRow() const noexcept { return m_startRow; }
        [[nodiscard]] std::size_t StartColumn() const noexcept { return m_startColumn; }

    private:
        NestedXpr& m_xpr;
        std::size_t m_startRow;
        std::size_t m_startColumn;
    };

    template<typename NestedXpr, std::size_t BlockRows, std::size_t BlockColumns, bool IsConst>
    class Evaluator<Block<NestedXpr, BlockRows, BlockColumns>, IsConst>
    {
        using Xpr = Block<NestedXpr, BlockRows, BlockColumns>;
        using InternalXpr = std::conditional_t<IsConst, const Xpr, Xpr>;
        using Scalar = Traits<Xpr>::Scalar;

    public:
        explicit Evaluator(InternalXpr& block) : m_xprEvaluator(block.NestedExpression()),
            m_startRow(block.StartRow()), m_startColumn(block.StartColumn()),
            m_offset(HasFlag<Xpr, XprFlag::RowMajor> ? m_startRow * BlockColumns + m_startColumn :
                m_startColumn * BlockRows + m_startRow)
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

        Scalar& Entry(std::size_t row, std::size_t column) requires !IsConst
        {
            return m_xprEvaluator.Entry(m_startRow + row, m_startColumn + column);
        }

    private:
        Evaluator<std::remove_const_t<NestedXpr>, IsConst> m_xprEvaluator;
        std::size_t m_startRow;
        std::size_t m_startColumn;
        std::size_t m_offset;
    };
}
