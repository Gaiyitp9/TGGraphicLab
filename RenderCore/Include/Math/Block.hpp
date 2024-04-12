/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename Xpr, int BlockRows, int BlockColumns>
    struct Traits<Block<Xpr, BlockRows, BlockColumns>>
    {
        using Scalar = Traits<Xpr>::Scalar;
        static constexpr int        Rows = BlockRows;
        static constexpr int        Columns = BlockColumns;
        static constexpr int        Size = Rows * Columns;
        static constexpr XprFlag    Flags = (Traits<Xpr>::Flags & (XprFlag::RowMajor | XprFlag::DirectAccess)) |
                                            (Rows == 1 || Columns == 1 ? XprFlag::Vector : XprFlag::None) |
                                            (Rows == Columns ? XprFlag::Square : XprFlag::None);
    };

    template<typename Xpr, int BlockRows, int BlockColumns>
    class Block : public MatrixBase<Block<Xpr, BlockRows, BlockColumns>>
    {
    public:
        Block(const Xpr& xpr, int startRow, int startCol)
            : m_xpr(xpr), m_startRow(startRow), m_startCol(startCol) {}

        const Xpr& NestedExpression() const { return m_xpr; }
        [[nodiscard]] int StartRow() const { return m_startRow; }
        [[nodiscard]] int StartCol() const { return m_startCol; }

    private:
        const Xpr& m_xpr;
        int m_startRow;
        int m_startCol;
    };

    template<typename Xpr, int BlockRows, int BlockColumns,
            bool HasDirectAccess = (Traits<Xpr>::Flags & XprFlag::DirectAccess) != XprFlag::None>
    class BlockEvaluator;

    template<typename Xpr, int BlockRows, int BlockColumns>
    class Evaluator<Block<Xpr, BlockRows, BlockColumns>> : public BlockEvaluator<Xpr, BlockRows, BlockColumns>
    {
        using Base = BlockEvaluator<Xpr, BlockRows, BlockColumns>;
    public:
        explicit Evaluator(const Xpr& xpr, int startRow, int startCol) : Base(xpr, startRow, startCol) {}
    };

    template<typename Xpr, int BlockRows, int BlockColumns>
    class BlockEvaluator<Xpr, BlockRows, BlockColumns, false>
    {
    public:
        using XprType = Block<Xpr, BlockRows, BlockColumns>;
        using CoeffType = Traits<XprType>::Scalar;

        explicit BlockEvaluator(const XprType& block, int startRow, int startCol)
            : m_xprEvaluator(block.NestedExpression()), m_startRow(startRow), m_startCol(startCol),
            m_offset((Traits<XprType>::Flags & XprFlag::RowMajor) != XprFlag::None ?
                    startRow * BlockColumns + startCol :
                    startCol * BlockRows + startRow)
        {}

        CoeffType Coefficient(int index) const
        {
            return m_xprEvaluator.Coefficient(m_offset + index);
        }

        CoeffType Coefficient(int row, int col) const
        {
            return m_xprEvaluator.Coefficient(m_startRow + row, m_startCol + col);
        }

    protected:
        Evaluator<Xpr> m_xprEvaluator;
        int m_startRow;
        int m_startCol;
        int m_offset;
    };

    template<typename Xpr, int BlockRows, int BlockColumns>
    class BlockEvaluator<Xpr, BlockRows, BlockColumns, true> : public BlockEvaluator<Xpr, BlockRows, BlockColumns, false>
    {
        using Base = BlockEvaluator<Xpr, BlockRows, BlockColumns, false>;
        using XprType = Base::XprType;
        using CoeffType = Base::CoeffType;
//        using Base::m_xprEvaluator;

    public:
        explicit BlockEvaluator(const XprType& block, int startRow, int startCol)
            : Base(block, startRow, startCol) {}

        CoeffType& CoefficientRef(int index)
        {
//            m_xprEvaluator.CoefficientRef(index);
        }
    };
}
