/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename NestedXpr, std::size_t BlockRows, std::size_t BlockColumns>
    struct Traits<Block<NestedXpr, BlockRows, BlockColumns>>
    {
    private:
        // Block可以线性访问的条件：
        // 1. 内嵌表达式可以线性访问
        // 2. 对于row major的表达式，Block的列数等于内嵌表达式的列数或者Block行数等于1;
        // 对于column major的表达式，Block的行数等于内嵌表达式的行数或者Block列数等于1
        static constexpr bool CanLinearAccess = CheckFlag<NestedXpr>(XprFlag::LinearAccess) && (
                CheckFlag<NestedXpr>(XprFlag::RowMajor) ? (BlockColumns == Traits<NestedXpr>::Columns || BlockRows == 1) :
                (BlockRows == Traits<NestedXpr>::Rows || BlockColumns == 1));

    public:
        using Scalar = Traits<NestedXpr>::Scalar;
        static constexpr std::size_t    Rows = BlockRows;
        static constexpr std::size_t    Columns = BlockColumns;
        static constexpr std::size_t    Size = Rows * Columns;
        static constexpr XprFlag        Flags = (Traits<NestedXpr>::Flags & (XprFlag::RowMajor | XprFlag::LeftValue)) |
                (CanLinearAccess ? XprFlag::LinearAccess : XprFlag::None) |
                (Rows == 1 || Columns == 1 ? XprFlag::Vector : XprFlag::None) |
                (Rows == Columns ? XprFlag::Square : XprFlag::None);
    };

    template<typename NestedXpr, std::size_t BlockRows, std::size_t BlockColumns>
    class Block : public MatrixBase<Block<NestedXpr, BlockRows, BlockColumns>>
    {
    public:
        Block(const NestedXpr& xpr, std::size_t startRow, std::size_t startColumn)
            : m_xpr(xpr), m_startRow(startRow), m_startColumn(startColumn) {}

        const NestedXpr& NestedExpression() const { return m_xpr; }
        [[nodiscard]] std::size_t StartRow() const { return m_startRow; }
        [[nodiscard]] std::size_t startColumn() const { return m_startColumn; }

    private:
        const NestedXpr& m_xpr;
        std::size_t m_startRow;
        std::size_t m_startColumn;
    };

    template<typename NestedXpr, std::size_t BlockRows, std::size_t BlockColumns,
            bool IsLeftValue = CheckFlag<NestedXpr>(XprFlag::LeftValue)>
    class BlockEvaluator;

    template<typename NestedXpr, std::size_t BlockRows, std::size_t BlockColumns>
    class Evaluator<Block<NestedXpr, BlockRows, BlockColumns>> : public BlockEvaluator<NestedXpr, BlockRows, BlockColumns>
    {
        using Base = BlockEvaluator<NestedXpr, BlockRows, BlockColumns>;

    public:
        using XprType = Block<NestedXpr, BlockRows, BlockColumns>;
        using CoeffType = Traits<XprType>::Scalar;

        explicit Evaluator(const XprType& block) : Base(block, block.StartRow(), block.startColumn()) {}
    };

    template<typename NestedXpr, std::size_t BlockRows, std::size_t BlockColumns>
    class BlockEvaluator<NestedXpr, BlockRows, BlockColumns, false>
    {
    public:
        using XprType = Block<NestedXpr, BlockRows, BlockColumns>;
        using CoeffType = Traits<XprType>::Scalar;

        BlockEvaluator(const XprType& block, std::size_t startRow, std::size_t startColumn)
            : m_xprEvaluator(block.NestedExpression()), m_startRow(startRow), m_startColumn(startColumn),
            m_offset(CheckFlag<XprType>(XprFlag::RowMajor) ? startRow * Traits<NestedXpr>::Columns + startColumn :
                    startColumn * Traits<NestedXpr>::Rows + startRow)
        {}

        [[nodiscard]] CoeffType Coefficient(std::size_t index) const
        {
            return m_xprEvaluator.Coefficient(m_offset + index);
        }

        [[nodiscard]] CoeffType Coefficient(std::size_t row, std::size_t column) const
        {
            return m_xprEvaluator.Coefficient(m_startRow + row, m_startColumn + column);
        }

    protected:
        Evaluator<NestedXpr> m_xprEvaluator;
        std::size_t m_startRow;
        std::size_t m_startColumn;
        std::size_t m_offset;
    };

    template<typename NestedXpr, std::size_t BlockRows, std::size_t BlockColumns>
    class BlockEvaluator<NestedXpr, BlockRows, BlockColumns, true> : public BlockEvaluator<NestedXpr, BlockRows, BlockColumns, false>
    {
        using Base = BlockEvaluator<NestedXpr, BlockRows, BlockColumns, false>;
        using XprType = Base::XprType;
        using CoeffType = Base::CoeffType;
        using Base::m_xprEvaluator;
        using Base::m_offset;
        using Base::m_startRow;
        using Base::m_startColumn;

    public:
        BlockEvaluator(const XprType& block, std::size_t startRow, std::size_t startColumn)
            : Base(block, startRow, startColumn) {}

        CoeffType& CoefficientRef(std::size_t index)
        {
            return m_xprEvaluator.CoefficientRef(m_offset + index);
        }

        CoeffType& CoefficientRef(std::size_t row, std::size_t column)
        {
            return m_xprEvaluator.CoefficientRef(m_startRow + row, m_startColumn + column);
        }
    };
}
