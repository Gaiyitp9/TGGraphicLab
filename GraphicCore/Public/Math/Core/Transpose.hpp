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

        [[nodiscard]] const Xpr& NestedExpression() const noexcept { return m_xpr; }

    private:
        NestedXpr m_xpr;
    };
}
