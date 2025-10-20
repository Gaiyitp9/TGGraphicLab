/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename LhsXpr, typename RhsXpr, ProductType Type>
    struct Traits<Product<LhsXpr, RhsXpr, Type>>
    {
    private:
        static constexpr bool IsSameOrder = HasFlag<LhsXpr, XprFlag::RowMajor> == HasFlag<RhsXpr, XprFlag::RowMajor>;
        static constexpr XprFlag Order = DefaultOrder == StorageOrder::RowMajor ? XprFlag::RowMajor : XprFlag::None;
    public:
        using Scalar = Traits<LhsXpr>::Scalar;
        static constexpr std::size_t    Rows = Traits<LhsXpr>::Rows;
        static constexpr std::size_t	Columns = Traits<RhsXpr>::Columns;
        static constexpr std::size_t	Size = Rows * Columns;
        static constexpr XprFlag        Flags = (IsSameOrder ? Traits<LhsXpr>::Flags & XprFlag::RowMajor
            : Order) | XprFlag::LinearAccess;
    };

    // 矩阵乘法表达式
	template<typename LhsXpr, typename RhsXpr, ProductType Type> requires MatrixMultipliable<LhsXpr, RhsXpr>
	class Product final : public MatrixBase<Product<LhsXpr, RhsXpr, Type>>
	{
	    using NestedLhsXpr = RefSelector<LhsXpr>::Type;
	    using NestedRhsXpr = RefSelector<RhsXpr>::Type;

    public:
        Product(const LhsXpr& lhs, const RhsXpr& rhs) : m_lhs(lhs), m_rhs(rhs) {}

        const LhsXpr& LhsExpression() const { return m_lhs; }
        const RhsXpr& RhsExpression() const { return m_rhs; }

    private:
        NestedLhsXpr m_lhs;
        NestedRhsXpr m_rhs;
	};
}
