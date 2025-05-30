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
	template<typename BinaryOp, typename LhsXpr, typename RhsXpr>
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

    // 矩阵逐元素二元运算，要求矩阵元素类型相同以及行列相等
    template<typename LhsXpr, typename RhsXpr>
    concept CWiseOperable =
        std::convertible_to<typename Traits<LhsXpr>::Scalar, typename Traits<RhsXpr>::Scalar> &&
        Traits<LhsXpr>::Rows == Traits<RhsXpr>::Rows &&
        Traits<LhsXpr>::Columns == Traits<RhsXpr>::Columns;

    template<typename BinaryOp, typename LhsXpr, typename RhsXpr> requires CWiseOperable<LhsXpr, RhsXpr>
    class Evaluator<CWiseBinaryOp<BinaryOp, LhsXpr, RhsXpr>>
    {
        using Xpr = CWiseBinaryOp<BinaryOp, LhsXpr, RhsXpr>;
        using Scalar = Traits<Xpr>::Scalar;

    public:
        explicit Evaluator(const Xpr& xpr) : m_functor(xpr.Functor()),
            m_lhsEvaluator(xpr.LhsExpression()), m_rhsEvaluator(xpr.RhsExpression()) {}

        [[nodiscard]] Scalar Entry(std::size_t index) const requires HasFlag<Xpr, XprFlag::LinearAccess>
        {
            return m_functor(m_lhsEvaluator.Entry(index), m_rhsEvaluator.Entry(index));
        }
        [[nodiscard]] Scalar Entry(std::size_t row, std::size_t col) const
        {
            return m_functor(m_lhsEvaluator.Entry(row, col), m_rhsEvaluator.Entry(row, col));
        }

    private:
        const BinaryOp m_functor;
        Evaluator<LhsXpr> m_lhsEvaluator;
        Evaluator<RhsXpr> m_rhsEvaluator;
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
    CWiseBinaryOp<ScalarProductOp<typename Traits<Derived>::Scalar>, Derived,
        CWiseNullaryOp<ScalarConstantOp<typename Traits<Derived>::Scalar>, Derived>>
    operator*(typename Traits<Derived>::Scalar scalar, const Derived& other)
    {
        return other * scalar;
    }
}
