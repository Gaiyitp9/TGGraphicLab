/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename NestedXpr, std::size_t StartRow_, std::size_t StartColumn_, std::size_t BlockRows,
        std::size_t BlockColumns>
    struct Traits<Block<NestedXpr, StartRow_, StartColumn_, BlockRows, BlockColumns>>
    {
    private:
        // Block可以线性访问的条件：
        // 1. 内嵌表达式可以线性访问
        // 2. 对于row major的表达式，Block的列数等于内嵌表达式的列数或者Block行数等于1;
        // 对于column major的表达式，Block的行数等于内嵌表达式的行数或者Block列数等于1
        static constexpr bool LinearAccessible = HasFlag<NestedXpr, XprFlag::LinearAccess> && (
                HasFlag<NestedXpr, XprFlag::RowMajor> ? BlockColumns == Traits<NestedXpr>::Columns || BlockRows == 1 :
                BlockRows == Traits<NestedXpr>::Rows || BlockColumns == 1);

    public:
        using Scalar = Traits<NestedXpr>::Scalar;
        static constexpr std::size_t    Rows = BlockRows;
        static constexpr std::size_t    Columns = BlockColumns;
        static constexpr std::size_t    Size = Rows * Columns;
        static constexpr XprFlag        Flags = Traits<NestedXpr>::Flags & (LinearAccessible ? ~XprFlag::None :
            ~XprFlag::LinearAccess);
    };

    template<typename NestedXpr, std::size_t StartRow_, std::size_t StartColumn_, std::size_t BlockRows,
        std::size_t BlockColumns>
    class Block : public MatrixBase<Block<NestedXpr, StartRow_, StartColumn_, BlockRows, BlockColumns>>
    {
    public:
        explicit Block(NestedXpr& xpr) : m_xpr(xpr) {}

        NestedXpr& NestedExpression() noexcept { return m_xpr; }
        const NestedXpr& NestedExpression() const noexcept { return m_xpr; }

        static constexpr std::size_t StartRow() { return StartRow_; }
        static constexpr std::size_t StartColumn() { return StartColumn_; }

    private:
        NestedXpr& m_xpr;
    };

    template<typename NestedXpr, std::size_t StartRow, std::size_t StartColumn, std::size_t BlockRows,
        std::size_t BlockColumns, bool IsConst>
    class Evaluator<Block<NestedXpr, StartRow, StartColumn, BlockRows, BlockColumns>, IsConst>
    {
        using Xpr = Block<NestedXpr, StartRow, StartColumn, BlockRows, BlockColumns>;
        using InternalXpr = std::conditional_t<IsConst, const Xpr, Xpr>;
        using Scalar = Traits<Xpr>::Scalar;
        static constexpr std::size_t Offset = HasFlag<Xpr, XprFlag::RowMajor> ?
                StartRow * BlockColumns + StartColumn : StartColumn * BlockRows + StartRow;

    public:
        explicit Evaluator(InternalXpr& block) : m_xprEvaluator(block.NestedExpression()) {}

        Scalar Entry(std::size_t index) const requires HasFlag<Xpr, XprFlag::LinearAccess>
        {
            return m_xprEvaluator.Entry(Offset + index);
        }

        Scalar Entry(std::size_t row, std::size_t column) const
        {
            return m_xprEvaluator.Entry(StartRow + row, StartColumn + column);
        }

        Scalar& Entry(std::size_t index) requires (HasFlag<Xpr, XprFlag::LinearAccess> && !IsConst)
        {
            return m_xprEvaluator.Entry(Offset + index);
        }

        Scalar& Entry(std::size_t row, std::size_t column) requires !IsConst
        {
            return m_xprEvaluator.Entry(StartRow + row, StartColumn + column);
        }

    private:
        Evaluator<NestedXpr, IsConst> m_xprEvaluator;
    };
}
