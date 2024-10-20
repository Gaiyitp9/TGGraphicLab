/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "MatrixBase.hpp"

namespace TG::Math
{
    template<typename NestedXpr>
    struct Traits<Inverse<NestedXpr>>
    {
        using Scalar = Traits<NestedXpr>::Scalar;
        static constexpr std::size_t    Rows = Traits<NestedXpr>::Rows;
        static constexpr std::size_t    Columns = Traits<NestedXpr>::Columns;
        static constexpr std::size_t    Size = Rows * Columns;
        // 移除左值标志，添加线性访问标志
        static constexpr XprFlag        Flags = Traits<NestedXpr>::Flags & ~XprFlag::LeftValue | XprFlag::LinearAccess;
    };

    template<typename NestedXpr>
    class Inverse final : public MatrixBase<Inverse<NestedXpr>>
    {
    public:
        explicit Inverse(const NestedXpr& xpr) : m_xpr(xpr) {}

        [[nodiscard]] const NestedXpr& NestedExpression() const noexcept { return m_xpr; }

    private:
        const NestedXpr& m_xpr;
    };

    template<typename NestedXpr>
    class Evaluator<Inverse<NestedXpr>>
    {
        using Xpr = Inverse<NestedXpr>;
        using Scalar = Traits<Xpr>::Scalar;

    public:
        explicit Evaluator(const Xpr& xpr)
        {
            // 计算逆矩阵并把结果储存在m_inverse中
            CallAssignmentNoAlias(m_inverse, xpr.NestedExpression());
        }

        Scalar Entry(std::size_t index) const
        {
            return m_inverse[index];
        }

        Scalar Entry(std::size_t row, std::size_t col) const
        {
            return m_inverse(row, col);
        }

    private:
        PlainMatrix<Xpr> m_inverse;
    };

    template<typename Dst, typename Src, std::size_t Order>
    class InverseImpl
    {
        using Scalar = Traits<Src>::Scalar;
        using Matrix = PlainMatrix<Src>;

    public:
        void operator()(Dst& dst, const Matrix& src) const requires (Order == 1)
        {
            RefEvaluator<Dst> dstEvaluator(dst);
            dstEvaluator.Entry(0, 0) = static_cast<Scalar>(1) / src(0, 0);
        }

        void operator()(Dst& dst, const Matrix& src) const requires (Order == 2)
        {
            RefEvaluator<Dst> dstEvaluator(dst);
            Scalar invDet = static_cast<Scalar>(1) / src.Determinant();
            // 避免aliasing问题
            Scalar temp = src(0, 0);
            // 逆矩阵的伴随矩阵除以行列式
            dstEvaluator.Entry(0, 0) = src(1, 1) * invDet;
            dstEvaluator.Entry(1, 0) = -src(1, 0) * invDet;
            dstEvaluator.Entry(0, 1) = -src(0, 1) * invDet;
            dstEvaluator.Entry(1, 1) = temp * invDet;
        }

        void operator()(Dst& dst, const Matrix& src) const requires (Order == 3)
        {
            RefEvaluator<Dst> dstEvaluator(dst);
            Scalar invDet = static_cast<Scalar>(1) / src.Determinant();
            Scalar c00 = Cofactor3X3(src, 0, 0);
            Scalar c01 = Cofactor3X3(src, 0, 1);
            Scalar c02 = Cofactor3X3(src, 0, 2);
            Scalar c10 = Cofactor3X3(src, 1, 0);
            Scalar c11 = Cofactor3X3(src, 1, 1);
            Scalar c20 = Cofactor3X3(src, 2, 0);
            // 更新这三个值不会造成aliasing问题
            dstEvaluator.Entry(1, 2) = Cofactor3X3(src, 2, 1) * invDet;
            dstEvaluator.Entry(2, 1) = Cofactor3X3(src, 1, 2) * invDet;
            dstEvaluator.Entry(2, 2) = Cofactor3X3(src, 2, 2) * invDet;
            dstEvaluator.Entry(0, 0) = c00 * invDet;
            dstEvaluator.Entry(0, 1) = c10 * invDet;
            dstEvaluator.Entry(0, 2) = c20 * invDet;
            dstEvaluator.Entry(1, 0) = c01 * invDet;
            dstEvaluator.Entry(1, 1) = c11 * invDet;
            dstEvaluator.Entry(2, 0) = c02 * invDet;
        }

        void operator()(Dst& dst, const Matrix& src) const requires (Order == 4)
        {
            RefEvaluator<Dst> dstEvaluator(dst);
            Scalar invDet = static_cast<Scalar>(1) / src.Determinant();
            
            dstEvaluator.Entry(0, 0) = Minor4X4(src, 0, 0) * invDet;
            dstEvaluator.Entry(1, 0) = -Minor4X4(src, 0, 1) * invDet;
            dstEvaluator.Entry(2, 0) = Minor4X4(src, 0, 2) * invDet;
            dstEvaluator.Entry(3, 0) = -Minor4X4(src, 0, 3) * invDet;
            dstEvaluator.Entry(0, 2) = Minor4X4(src, 2, 0) * invDet;
            dstEvaluator.Entry(1, 2) = -Minor4X4(src, 2, 1) * invDet;
            dstEvaluator.Entry(2, 2) = Minor4X4(src, 2, 2) * invDet;
            dstEvaluator.Entry(3, 2) = -Minor4X4(src, 2, 3) * invDet;
            dstEvaluator.Entry(0, 1) = -Minor4X4(src, 1, 0) * invDet;
            dstEvaluator.Entry(1, 1) = Minor4X4(src, 1, 1) * invDet;
            dstEvaluator.Entry(2, 1) = -Minor4X4(src, 1, 2) * invDet;
            dstEvaluator.Entry(3, 1) = Minor4X4(src, 1, 3) * invDet;
            dstEvaluator.Entry(0, 3) = -Minor4X4(src, 3, 0) * invDet;
            dstEvaluator.Entry(1, 3) = Minor4X4(src, 3, 1) * invDet;
            dstEvaluator.Entry(2, 3) = -Minor4X4(src, 3, 2) * invDet;
            dstEvaluator.Entry(3, 3) = Minor4X4(src, 3, 3) * invDet;
        }

        void operator()(Dst& dst, const Matrix& src) const
        {

        }

    private:
        // 第row行第column列的代数余子式
        static Scalar Cofactor3X3(const Matrix& mat, std::size_t row, std::size_t column)
        {
            std::size_t row0 = (row + 1) % 3;
            std::size_t row1 = (row + 2) % 3;
            std::size_t column0 = (column + 1) % 3;
            std::size_t column1 = (column + 2) % 3;
            // 注意，这里的计算公式已经考虑了(-1)^(row + column)
            // 交换行列式的两行（或两列），行列式的值会改变符号。
            // 结合这个性质去理解，row0和column0并不是一定是第一行，比如row等于1时, row0等于2, row1等于0，相当于交换了两行，
            // 所以此时计算的行列式是负的，正好等于(-1)^(row + column)
            return mat(row0, column0) * mat(row1, column1) - mat(row0, column1) * mat(row1, column0);
        }

        static Scalar Det3Helper(const Matrix& mat, std::size_t row0, std::size_t row1, std::size_t row2,
            std::size_t column0, std::size_t Column1, std::size_t Column2)
        {
            // 代数余子式求行列式公式的一部分。计算过程中考虑了(-1)^(row0 + column0)，可参考Cofactor3X3的分析
            return mat(row0, column0) *
                (mat(row1, Column1) * mat(row2, Column2) - mat(row1, Column2) * mat(row2, Column1));
        }
        // 第row行第column列的余子式
        static Scalar Minor4X4(const Matrix& mat, std::size_t row, std::size_t column)
        {
            std::size_t row0 = (row + 1) % 4;
            std::size_t row1 = (row + 2) % 4;
            std::size_t row2 = (row + 3) % 4;
            std::size_t column0 = (column + 1) % 4;
            std::size_t column1 = (column + 2) % 4;
            std::size_t column2 = (column + 3) % 4;
            // 注意，这里的计算公式没有考虑了(-1)^(row + column)，因为下面的式子是求行列式，没有把系数包含进去
            return Det3Helper(mat, row0, row1, row2, column0, column1, column2) +
                Det3Helper(mat, row1, row2, row0, column0, column1, column2) +
                    Det3Helper(mat, row2, row0, row1, column0, column1, column2);
        }
    };

    // 默认矩阵乘法的赋值，可以避免创建Evaluator<Product<LhsXpr, RhsXpr, Default>>，即避免创建一个矩阵临时变量
    template<typename Dst, typename NestedXpr, typename AssignFunctor>
    class Assignment<Dst, Inverse<NestedXpr>, AssignFunctor>
    {
        using Src = Inverse<NestedXpr>;
    public:
        void operator()(Dst& dst, const Src& src, AssignFunctor functor) const
        {
             InverseImpl<Dst, Src, Traits<Src>::Rows>{}(dst, src.NestedExpression());
        }
    };
}
