/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename NestedXpr, int BlockRows, int BlockColumns>
    struct Traits<Block<NestedXpr, BlockRows, BlockColumns>>
    {
        using Scalar = Traits<NestedXpr>::Scalar;
        static constexpr int        Rows = BlockRows;
        static constexpr int        Columns = BlockColumns;
        static constexpr int        Size = Rows * Columns;
        static constexpr XprFlag    Flags = (Traits<NestedXpr>::Flags & (XprFlag::RowMajor | XprFlag::DirectAccess)) |
                (CheckFlag<NestedXpr>(XprFlag::Vector) && CheckFlag<NestedXpr>(XprFlag::LinearAccess) ?
                XprFlag::LinearAccess : XprFlag::None) |
                (Rows == 1 || Columns == 1 ? XprFlag::Vector : XprFlag::None) |
                (Rows == Columns ? XprFlag::Square : XprFlag::None);
    };

    template<typename NestedXpr, int BlockRows, int BlockColumns>
    class Block : public MatrixBase<Block<NestedXpr, BlockRows, BlockColumns>>
    {
    public:
        Block(const NestedXpr& xpr, int startRow, int startCol)
            : m_xpr(xpr), m_startRow(startRow), m_startCol(startCol) {}

        const NestedXpr& NestedExpression() const { return m_xpr; }
        [[nodiscard]] int StartRow() const { return m_startRow; }
        [[nodiscard]] int StartCol() const { return m_startCol; }

    private:
        const NestedXpr& m_xpr;
        int m_startRow;
        int m_startCol;
    };

    template<typename NestedXpr, int BlockRows, int BlockColumns,
            bool HasDirectAccess = CheckFlag<NestedXpr>(XprFlag::DirectAccess)>
    class BlockEvaluator;

    template<typename NestedXpr, int BlockRows, int BlockColumns>
    class Evaluator<Block<NestedXpr, BlockRows, BlockColumns>> : public BlockEvaluator<NestedXpr, BlockRows, BlockColumns>
    {
        using Base = BlockEvaluator<NestedXpr, BlockRows, BlockColumns>;

    public:
        using XprType = Block<NestedXpr, BlockRows, BlockColumns>;
        using CoeffType = Traits<XprType>::Scalar;

        explicit Evaluator(const XprType& block) : Base(block, block.StartRow(), block.StartCol()) {}
    };

    template<typename NestedXpr, int BlockRows, int BlockColumns>
    class BlockEvaluator<NestedXpr, BlockRows, BlockColumns, false>
    {
    public:
        using XprType = Block<NestedXpr, BlockRows, BlockColumns>;
        using CoeffType = Traits<XprType>::Scalar;

        BlockEvaluator(const XprType& block, int startRow, int startCol)
            : m_xprEvaluator(block.NestedExpression()), m_startRow(startRow), m_startCol(startCol),
            m_offset(CheckFlag<XprType>(XprFlag::RowMajor) ? startRow * Traits<NestedXpr>::Columns + startCol :
                    startCol * Traits<NestedXpr>::Rows + startRow)
        {}

        [[nodiscard]] CoeffType Coefficient(int index) const
        {
            return m_xprEvaluator.Coefficient(m_offset + index);
        }

        [[nodiscard]] CoeffType Coefficient(int row, int col) const
        {
            return m_xprEvaluator.Coefficient(m_startRow + row, m_startCol + col);
        }

    protected:
        Evaluator<NestedXpr> m_xprEvaluator;
        int m_startRow;
        int m_startCol;
        int m_offset;
    };

    template<typename NestedXpr, int BlockRows, int BlockColumns>
    class BlockEvaluator<NestedXpr, BlockRows, BlockColumns, true> : public BlockEvaluator<NestedXpr, BlockRows, BlockColumns, false>
    {
        using Base = BlockEvaluator<NestedXpr, BlockRows, BlockColumns, false>;
        using XprType = Base::XprType;
        using CoeffType = Base::CoeffType;
        using Base::m_xprEvaluator;
        using Base::m_offset;
        using Base::m_startRow;
        using Base::m_startCol;

    public:
        BlockEvaluator(const XprType& block, int startRow, int startCol)
            : Base(block, startRow, startCol) {}

        CoeffType& CoefficientRef(int index)
        {
            return m_xprEvaluator.CoefficientRef(m_offset + index);
        }

        CoeffType& CoefficientRef(int row, int col)
        {
            return m_xprEvaluator.CoefficientRef(m_startRow + row, m_startCol + col);
        }
    };
}
