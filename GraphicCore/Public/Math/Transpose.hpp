/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename Xpr>
    struct Traits<Transpose<Xpr>>
    {
        using Scalar = Traits<Xpr>::Scalar;
        static constexpr std::size_t    Rows = Traits<Xpr>::Columns;
        static constexpr std::size_t    Columns = Traits<Xpr>::Rows;
        static constexpr std::size_t    Size = Rows * Columns;
        // 1.存储顺序翻转 2.非左值 3.值嵌入，其他的标志不变
        static constexpr XprFlag        Flags = (Traits<Xpr>::Flags ^ XprFlag::RowMajor) &
            ~XprFlag::LeftValue & ~XprFlag::NestByRef;
    };

    template<typename Xpr>
    class Transpose final : public MatrixBase<Transpose<Xpr>>
    {
        using NestedXpr = RefSelector<Xpr>::Type;
        using Base = MatrixBase<Transpose>;

    public:
        explicit Transpose(const Xpr& xpr) : m_xpr(xpr) {}

        // 解决MatrixBase的Transpose方法和Transpose类重名问题
        // 不添加using声明时，Transpose对象无法调用基类的Transpose方法，会识别为类名
        using Base::Transpose;

        [[nodiscard]] const Xpr& NestedExpression() const noexcept { return m_xpr; }

    private:
        NestedXpr m_xpr;
    };

    template<typename ArgXpr>
    class Evaluator<Transpose<ArgXpr>>
    {
        using Xpr = Transpose<ArgXpr>;
        using Scalar = Traits<Xpr>::Scalar;

    public:
        explicit Evaluator(const Xpr& transpose) : m_xprEvaluator(transpose.NestedExpression()) {}

        [[nodiscard]] Scalar Entry(std::size_t index) const requires HasFlag<Xpr, XprFlag::LinearAccess>
        {
            return m_xprEvaluator.Entry(index);
        }

        [[nodiscard]] Scalar Entry(std::size_t row, std::size_t column) const
        {
            return m_xprEvaluator.Entry(column, row);
        }

    private:
        Evaluator<ArgXpr> m_xprEvaluator;
    };
}
