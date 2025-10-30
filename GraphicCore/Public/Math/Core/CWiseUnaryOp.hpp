/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename UnaryOp, typename Xpr>
    struct Traits<CWiseUnaryOp<UnaryOp, Xpr>>
    {
        using Scalar = Traits<Xpr>::Scalar;
        static constexpr std::size_t	Rows = Traits<Xpr>::Rows;
        static constexpr std::size_t	Columns = Traits<Xpr>::Columns;
        static constexpr std::size_t	Size = Rows * Columns;
        static constexpr XprFlag        Flags = Traits<Xpr>::Flags & ~XprFlag::LeftValue & ~XprFlag::NestByRef;
    };

    template<typename UnaryOp, typename Xpr>
    class CWiseUnaryOp : public MatrixBase<CWiseUnaryOp<UnaryOp, Xpr>>
    {
	    using NestedXpr = RefSelector<Xpr>::Type;

    public:
        CWiseUnaryOp(const Xpr& xpr, UnaryOp nullaryOp)
            : m_unaryOp(nullaryOp), m_nestedXpr(xpr)
        {}

        [[nodiscard]] UnaryOp Functor() const { return m_unaryOp; }
        [[nodiscard]] const NestedXpr& NestedExpression() const noexcept { return m_nestedXpr; }

    private:
        UnaryOp m_unaryOp;
        NestedXpr m_nestedXpr;
    };

    template<typename Scalar>
    struct ScalarOppositeOp
    {
        Scalar operator()(Scalar a) const
        {
            return -a;
        }
    };
}
