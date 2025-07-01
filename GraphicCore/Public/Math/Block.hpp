/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    template<typename Xpr, std::size_t BlockRows, std::size_t BlockColumns>
    struct Traits<Block<Xpr, BlockRows, BlockColumns>>
    {
    private:
        // Block可以线性访问的条件：
        // 1. 内嵌表达式可以线性访问
        // 2. 对于row major的表达式，Block的列数等于内嵌表达式的列数或者Block行数等于1;
        // 对于column major的表达式，Block的行数等于内嵌表达式的行数或者Block列数等于1
        static constexpr bool LinearAccessible = HasFlag<Xpr, XprFlag::LinearAccess> &&
            (HasFlag<Xpr, XprFlag::RowMajor> ? BlockColumns == Traits<Xpr>::Columns || BlockRows == 1 :
                BlockRows == Traits<Xpr>::Rows || BlockColumns == 1);

    public:
        using Scalar = Traits<Xpr>::Scalar;
        static constexpr std::size_t    Rows = BlockRows;
        static constexpr std::size_t    Columns = BlockColumns;
        static constexpr std::size_t    Size = Rows * Columns;
        static constexpr XprFlag        Flags = Traits<Xpr>::Flags &
            (LinearAccessible ? ~XprFlag::None : ~XprFlag::LinearAccess) & ~XprFlag::NestByRef;
    };

    template<typename Xpr, std::size_t BlockRows, std::size_t BlockColumns>
    class Block final : public MatrixBase<Block<Xpr, BlockRows, BlockColumns>>
    {
        static constexpr bool IsConst = std::is_const_v<Xpr>;
        using RawXpr = std::remove_const_t<Xpr>;
        using NestedXpr = std::conditional_t<IsConst,
            typename RefSelector<RawXpr>::Type,
            typename RefSelector<RawXpr>::NonConstType>;
        using Base = MatrixBase<Block>;

    public:
        Block(Xpr& xpr, std::size_t startRow, std::size_t startColumn)
            : m_xpr(xpr), m_startRow(startRow), m_startColumn(startColumn) {}
        
        template<typename Derived> requires (!std::is_const_v<Xpr> && HasFlag<Block, XprFlag::LeftValue>)
        Block& operator=(const MatrixBase<Derived>& other)
        {
            CallAssignment(this->Expression(), other.Expression());
            return *this;
        }

        // 解决MatrixBase的Block方法和Block类重名问题
        // 不添加using声明时，Block对象无法调用基类的Block方法，会识别为类名
        using Base::Block;

        RawXpr& NestedExpression() noexcept requires !IsConst { return m_xpr; }
        [[nodiscard]] const RawXpr& NestedExpression() const noexcept { return m_xpr; }

        [[nodiscard]] std::size_t StartRow() const noexcept { return m_startRow; }
        [[nodiscard]] std::size_t StartColumn() const noexcept { return m_startColumn; }

    private:
        NestedXpr m_xpr;
        std::size_t m_startRow;
        std::size_t m_startColumn;
    };

    template<typename ArgXpr, std::size_t BlockRows, std::size_t BlockColumns, bool IsConst>
    class Evaluator<Block<ArgXpr, BlockRows, BlockColumns>, IsConst>
    {
        using Xpr = Block<ArgXpr, BlockRows, BlockColumns>;
        using InternalXpr = std::conditional_t<IsConst, const Xpr, Xpr>;
        using Scalar = Traits<Xpr>::Scalar;

    public:
        explicit Evaluator(InternalXpr& block) : m_xprEvaluator(block.NestedExpression()),
            m_startRow(block.StartRow()), m_startColumn(block.StartColumn()),
            m_offset(HasFlag<Xpr, XprFlag::RowMajor> ? m_startRow * Traits<ArgXpr>::Columns + m_startColumn :
                m_startColumn * Traits<ArgXpr>::Rows + m_startRow)
        {}

        [[nodiscard]] Scalar Entry(std::size_t index) const requires HasFlag<Xpr, XprFlag::LinearAccess>
        {
            return m_xprEvaluator.Entry(m_offset + index);
        }

        [[nodiscard]] Scalar Entry(std::size_t row, std::size_t column) const
        {
            return m_xprEvaluator.Entry(m_startRow + row, m_startColumn + column);
        }

        Scalar& Entry(std::size_t index) requires (HasFlag<Xpr, XprFlag::LinearAccess> && !IsConst)
        {
            return m_xprEvaluator.Entry(m_offset + index);
        }

        Scalar& Entry(std::size_t row, std::size_t column) requires !IsConst
        {
            return m_xprEvaluator.Entry(m_startRow + row, m_startColumn + column);
        }

    private:
        Evaluator<std::remove_const_t<ArgXpr>, IsConst> m_xprEvaluator;
        std::size_t m_startRow;
        std::size_t m_startColumn;
        std::size_t m_offset;
    };
}
