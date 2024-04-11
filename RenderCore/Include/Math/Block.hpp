/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename Xpr, int BlockRows, int BlockColumns>
    class Block : public MatrixBase<Block<Xpr, BlockRows, BlockColumns>>
    {
    public:
        Block(const Xpr& xpr, int startRow, int startCol)
            : m_xpr(xpr), m_startRow(startRow), m_startCol(startCol) {}

        const Xpr& NestedExpression() const { return m_xpr; }

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
    public:

    };

    template<typename Xpr, int BlockRows, int BlockColumns>
    class BlockEvaluator<Xpr, BlockRows, BlockColumns, false>
    {

    };

    template<typename Xpr, int BlockRows, int BlockColumns>
    class BlockEvaluator<Xpr, BlockRows, BlockColumns, true>
    {

    };
}
