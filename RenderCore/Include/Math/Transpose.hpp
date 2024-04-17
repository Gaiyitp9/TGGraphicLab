/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename NestedXpr>
    class Transpose : public MatrixBase<Transpose<NestedXpr>>
    {
    public:
        explicit Transpose(const NestedXpr& xpr) : m_xpr(xpr) {}

        const NestedXpr& NestedExpression() const { return m_xpr; }

    private:
        const NestedXpr& m_xpr;
    };
}
