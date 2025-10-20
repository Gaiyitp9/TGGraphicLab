/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename BinaryOp, typename LhsXpr, typename RhsXpr>
    struct Traits<CWiseBinaryOp<BinaryOp, LhsXpr, RhsXpr>>
    {
        using Scalar = Traits<LhsXpr>::Scalar;
        static constexpr std::size_t	Rows = Traits<LhsXpr>::Rows;
        static constexpr std::size_t	Columns = Traits<LhsXpr>::Columns;
        static constexpr std::size_t	Size = Rows * Columns;
        // 1.非左值 2.值嵌入 3.如果两个表达式存储顺序不同，则不能线性访问
        static constexpr XprFlag        Flags = Traits<LhsXpr>::Flags & Traits<RhsXpr>::Flags &
            ~XprFlag::LeftValue & ~XprFlag::NestByRef &
            (HasFlag<LhsXpr, XprFlag::RowMajor> == HasFlag<RhsXpr, XprFlag::RowMajor> ?
                ~XprFlag::None : ~XprFlag::LinearAccess);
    };

    // 二元表达式
	template<typename BinaryOp, typename LhsXpr, typename RhsXpr> requires CWiseOperable<LhsXpr, RhsXpr>
	class CWiseBinaryOp final : public MatrixBase<CWiseBinaryOp<BinaryOp, LhsXpr, RhsXpr>>
	{
	    using NestedLhsXpr = RefSelector<LhsXpr>::Type;
	    using NestedRhsXpr = RefSelector<RhsXpr>::Type;

    public:
        CWiseBinaryOp(const LhsXpr& lhs, const RhsXpr& rhs, BinaryOp op = {})
            : m_functor(op), m_lhs(lhs), m_rhs(rhs) {}

        [[nodiscard]] BinaryOp Functor() const noexcept { return m_functor; }
        [[nodiscard]] const LhsXpr& LhsExpression() const noexcept { return m_lhs; }
        [[nodiscard]] const RhsXpr& RhsExpression() const noexcept { return m_rhs; }

    private:
        const BinaryOp m_functor;
        NestedLhsXpr m_lhs;
        NestedRhsXpr m_rhs;
	};

    template<typename Scalar>
    struct ScalarAddOp
    {
        [[nodiscard]] Scalar operator()(Scalar a, Scalar b) const { return a + b; }
    };
    template<typename Scalar>
    struct ScalarSubtractOp
    {
        [[nodiscard]] Scalar operator()(Scalar a, Scalar b) const { return a - b; }
    };
    template<typename Scalar>
    struct ScalarProductOp
    {
        [[nodiscard]] Scalar operator()(Scalar a, Scalar b) const { return a * b; }
    };
    template<typename Scalar>
    struct ScalarDivideOp
    {
        [[nodiscard]] Scalar operator()(Scalar a, Scalar b) const { return a / b; }
    };

    template<typename Scalar, typename Functor>
    struct CompoundAssignOp
    {
        void operator()(Scalar& a, Scalar b)
        {
            AssignOp<Scalar>()(a, Functor()(a, b));
        }
    };
    template<typename Scalar>
    struct AddAssignOp : CompoundAssignOp<Scalar, ScalarAddOp<Scalar>> {};
    template<typename Scalar>
    struct SubtractAssignOp : CompoundAssignOp<Scalar, ScalarSubtractOp<Scalar>> {};

    // 常量左乘矩阵
    template<typename Derived> requires std::derived_from<Derived, MatrixBase<Derived>>
    CWiseBinaryOp<ScalarProductOp<typename Traits<Derived>::Scalar>,
        Derived,
        CWiseNullaryOp<ScalarConstantOp<typename Traits<Derived>::Scalar>, Derived>>
    operator*(typename Traits<Derived>::Scalar scalar, const Derived& other)
    {
        return other * scalar;
    }
}
