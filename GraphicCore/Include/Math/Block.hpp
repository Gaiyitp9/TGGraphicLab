/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename Xpr, std::size_t StartRow_, std::size_t StartColumn_, std::size_t BlockRows, std::size_t BlockColumns>
    struct Traits<Block<Xpr, BlockRows, BlockColumns>>
    {
    private:
        // Block可以线性访问的条件：
        // 1. 内嵌表达式可以线性访问
        // 2. 对于row major的表达式，Block的列数等于内嵌表达式的列数或者Block行数等于1;
        // 对于column major的表达式，Block的行数等于内嵌表达式的行数或者Block列数等于1
        static constexpr bool LinearAccessible = HasFlag<Xpr, XprFlag::LinearAccess> && (
                HasFlag<Xpr, XprFlag::RowMajor> ? BlockColumns == Traits<Xpr>::Columns || BlockRows == 1 :
                BlockRows == Traits<Xpr>::Rows || BlockColumns == 1);

    public:
        using Scalar = Traits<Xpr>::Scalar;
        static constexpr std::size_t    Rows = BlockRows;
        static constexpr std::size_t    Columns = BlockColumns;
        static constexpr std::size_t    Size = Rows * Columns;
        static constexpr XprFlag        Flags = Traits<Xpr>::Flags & (LinearAccessible ? ~XprFlag::None :
            ~XprFlag::LinearAccess);
        static constexpr std::size_t    StartRow = StartRow_;
        static constexpr std::size_t    StartColumn = StartColumn_;
    };

    template<typename Xpr, std::size_t StartRow, std::size_t StartColumn, std::size_t BlockRows, std::size_t BlockColumns>
    class Block : public MatrixBase<Block<Xpr, BlockRows, BlockColumns>>
    {
    public:
        Block(Xpr& xpr) : m_xpr(xpr) {}

        Xpr& NestedExpression() noexcept { return m_xpr; }
        const Xpr& NestedExpression() const noexcept { return m_xpr; }


    private:
        Xpr& m_xpr;
    };

    template<typename NestedXpr, std::size_t BlockRows, std::size_t BlockColumns>
    class Evaluator<const Block<NestedXpr, BlockRows, BlockColumns>>
    {
    public:
        using Xpr = Block<NestedXpr, BlockRows, BlockColumns>;

        explicit Evaluator(const Xpr& block) : m_xprEvaluator(block.NestedExpression()), m_startRow(block.StartRow()),
            m_startColumn(block.StartColumn())
        {}

        Traits<Xpr>::Scalar Entry(std::size_t index) requires HasFlag<Xpr, XprFlag::LinearAccess>
        {
            static constexpr std::size_t Offset = HasFlag<Xpr, XprFlag::RowMajor> ?
                Traits<Xpr>::S * Traits<Xpr>::Columns + m_startColumn :
                m_startColumn * Traits<Xpr>::Rows + m_startRow;
            return m_xprEvaluator.Entry(Offset + index);
        }

        Traits<Xpr>::Scalar Entry(std::size_t row, std::size_t column)
        {
            return m_xprEvaluator.Entry(m_startRow + row, m_startColumn + column);
        }

    private:
        Evaluator<NestedXpr> m_xprEvaluator;
        std::size_t m_startRow;
        std::size_t m_startColumn;
    };

    template<typename NestedXpr, std::size_t BlockRows, std::size_t BlockColumns>
    class Evaluator<Block<NestedXpr, BlockRows, BlockColumns>>
    {
    public:
        using Xpr = Block<NestedXpr, BlockRows, BlockColumns>;

        explicit Evaluator(Xpr& block) : m_xprEvaluator(block.NestedExpression()), m_startRow(block.StartRow()),
            m_startColumn(block.StartColumn()), m_offset(HasFlag<Xpr, XprFlag::RowMajor> ?
                m_startRow * Traits<NestedXpr>::Columns + m_startColumn :
                m_startColumn * Traits<NestedXpr>::Rows + m_startRow)
        {}

        Traits<Xpr>::Scalar& Entry(std::size_t index) requires HasFlag<Xpr, XprFlag::LinearAccess>
        {
            return m_xprEvaluator.Entry(m_offset + index);
        }

        Traits<Xpr>::Scalar& Entry(std::size_t row, std::size_t column)
        {
            return m_xprEvaluator.Entry(m_startRow + row, m_startColumn + column);
        }

    private:
        Evaluator<NestedXpr> m_xprEvaluator;
        std::size_t m_startRow;
        std::size_t m_startColumn;
        std::size_t m_offset;
    };
}
