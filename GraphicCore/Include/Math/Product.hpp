/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "MatrixBase.hpp"
#include "Assignment.hpp"

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
	template<typename LhsXpr, typename RhsXpr, ProductType Type>
	class Product final : public MatrixBase<Product<LhsXpr, RhsXpr, Type>>
	{
    public:
        Product(const LhsXpr& lhs, const RhsXpr& rhs) : m_lhs(lhs), m_rhs(rhs) {}

        const LhsXpr& LhsExpression() const { return m_lhs; }
        const RhsXpr& RhsExpression() const { return m_rhs; }

    private:
        const LhsXpr& m_lhs;
        const RhsXpr& m_rhs;
	};

    // 两个表达式是否可以执行矩阵乘法，左边表达式的列数要等于右边表达式的行数
    template<typename LhsXpr, typename RhsXpr>
    concept MatrixMultipliable = std::is_same_v<typename Traits<LhsXpr>::Scalar, typename Traits<RhsXpr>::Scalar> &&
            Traits<LhsXpr>::Columns == Traits<RhsXpr>::Rows;
    // 矩阵乘法默认类型需要考虑Aliasing
    template<typename LhsXpr, typename RhsXpr>
    constexpr bool EvaluatorAssumeAliasing<Product<LhsXpr, RhsXpr, ProductType::Default>> = true;

    // 矩阵乘法求值器
    template<typename LhsXpr, typename RhsXpr>  requires MatrixMultipliable<LhsXpr, RhsXpr>
    class Evaluator<Product<LhsXpr, RhsXpr, ProductType::Default>>
    {
        using Xpr = Product<LhsXpr, RhsXpr, ProductType::Default>;
        using Scalar = Traits<Xpr>::Scalar;

    public:
        explicit Evaluator(const Xpr& xpr)
        {
            // 计算矩阵乘法并把结果储存在m_product中
            CallAssignmentNoAlias(m_product, xpr.LhsExpression().LazyProduct(xpr.RhsExpression()));
        }

        Scalar Entry(std::size_t index) const
        {
            return m_product[index];
        }

        Scalar Entry(std::size_t row, std::size_t col) const
        {
            return m_product(row, col);
        }

    private:
        PlainMatrix<Xpr> m_product;
    };

    // 矩阵乘法求值器
    template<typename LhsXpr, typename RhsXpr>  requires MatrixMultipliable<LhsXpr, RhsXpr>
    class Evaluator<Product<LhsXpr, RhsXpr, ProductType::Lazy>>
    {
        using Xpr = Product<LhsXpr, RhsXpr, ProductType::Lazy>;
        using Scalar = Traits<Xpr>::Scalar;

    public:
        explicit Evaluator(const Xpr& xpr) : m_lhs(xpr.LhsExpression()), m_rhs(xpr.RhsExpression())
        {}

        Scalar Entry(std::size_t index) const
        {
            std::size_t row, column;
            if constexpr (HasFlag<Xpr, XprFlag::RowMajor>)
            {
                row = index / Traits<Xpr>::Columns;
                column = index % Traits<Xpr>::Columns;
            }
            else
            {
                row = index % Traits<Xpr>::Rows;
                column = index / Traits<Xpr>::Rows;
            }
            return Entry(row, column);
        }

        Scalar Entry(std::size_t row, std::size_t column) const
        {
            return m_lhs.Row(row).CWiseProduct(m_rhs.Column(column).transpose()).Sum();
        }

    private:
        const LhsXpr& m_lhs;
        const RhsXpr& m_rhs;
    };

    // 常量乘矩阵
    template<typename Derived> requires std::derived_from<Derived, MatrixBase<Derived>>
    Product<Derived, CWiseNullaryOp<ScalarConstantOp<typename Traits<Derived>::Scalar>, PlainMatrix<Derived>>,
        ProductType::Default> operator*(typename Traits<Derived>::Scalar scalar, const Derived& other)
    {
        return other * scalar;
    }

    // 默认矩阵乘法的赋值，可以避免创建Evaluator<Product<LhsXpr, RhsXpr, Default>>，即避免创建一个矩阵临时变量
    template<typename Dst, typename LhsXpr, typename RhsXpr, typename AssignFunctor>
    class Assignment<Dst, Product<LhsXpr, RhsXpr, ProductType::Default>, AssignFunctor>
    {
        using Src = Product<LhsXpr, RhsXpr, ProductType::Default>;
    public:
        void operator()(Dst& dst, const Src& src, AssignFunctor functor) const
        {
            CallAssignmentNoAlias(dst, src.LhsExpression().LazyProduct(src.RhsExpression()), functor);
        }
    };
}
