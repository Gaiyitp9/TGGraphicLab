/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <concepts>

namespace TG::Math
{
    // 给枚举添加逻辑运算
    template<typename Enum> requires std::is_enum_v<Enum>
    constexpr auto UnderlyingTypeCast(Enum e) { return static_cast<std::underlying_type_t<Enum>>(e); }

    template<typename Enum> requires std::is_enum_v<Enum>
    constexpr Enum operator|(Enum left, Enum right)
    {
        return static_cast<Enum>(UnderlyingTypeCast(left) | UnderlyingTypeCast(right));
    }

    template<typename Enum> requires std::is_enum_v<Enum>
    constexpr Enum operator^(Enum left, Enum right)
    {
        return static_cast<Enum>(UnderlyingTypeCast(left) ^ UnderlyingTypeCast(right));
    }

    template<typename Enum> requires std::is_enum_v<Enum>
    constexpr Enum operator&(Enum left, Enum right)
    {
        return static_cast<Enum>(UnderlyingTypeCast(left) & UnderlyingTypeCast(right));
    }

    template<typename Enum> requires std::is_enum_v<Enum>
    constexpr Enum operator~(Enum e)
    {
        return static_cast<Enum>(~UnderlyingTypeCast(e));
    }

    // 表达式特性，每种表达式都需要特化该类
    // 把表达式的特性定义在Traits里的原因：矩阵表达式基类使用CRTP(Curiously Recurring Template Pattern)，
    // 基类需要访问表达式的特性(比如Rows)，直接把特性定义在表达式中无法访问，因为在基类中的Derived是incomplete的
    // (我觉得这里是指派生类还未实例化，因为此时正在实例化基类)。可以使用Traits<Derived>来访问。
    // https://stackoverflow.com/questions/6006614/c-static-polymorphism-crtp-and-using-typedefs-from-derived-classes
    template<typename Xpr> struct Traits;
    // 表达式求值器，每种表达式都需要特化该类
    template<typename Xpr> class Evaluator;

    // 矩阵表达式标志
    enum class XprFlag : unsigned char
    {
        None            = 0,
        RowMajor        = 1,            // 按行储存
        LeftValue       = 1 << 1,       // 表达式是否是左值，也就是求值器有CoefficientRef函数
        LinearAccess    = 1 << 2,       // 是否能看作一维向量，也就是求值器可以调用Coefficient(index)函数
    };
    // 表达式标志是否包含指定标志
    template<typename Xpr, XprFlag flag> requires requires { Traits<Xpr>::Flags; }
    consteval bool ContainFlag()
    {
        return (Traits<Xpr>::Flags & flag) != XprFlag::None;
    }

    // 赋值遍历类型
    enum class Traversal : unsigned char
    {
        Default,
        Linear,
        Vectorized,
    };

	// 矩阵储存顺序
	enum class StorageOrder : unsigned char
	{
		RowMajor,
		ColumnMajor,
	};
    // 默认储存顺序，可以使用宏定义修改
#ifdef TG_ROW_MAJOR_MATRIX
	inline constexpr StorageOrder DefaultOrder = StorageOrder::RowMajor;
#else
	inline constexpr StorageOrder DefaultOrder = StorageOrder::ColumnMajor;
#endif

    // 矩阵表达式概念，分为两部分: 1. 矩阵表达式的特性 2. 表达式求值器的要求
    template<typename Xpr>
    concept IsMatrixExpression = requires
    {
        typename Traits<Xpr>::Scalar;
        Traits<Xpr>::Rows;
        Traits<Xpr>::Columns;
        Traits<Xpr>::Size;
        Traits<Xpr>::Flags;
    } &&
    std::constructible_from<Evaluator<Xpr>, const Xpr&> &&
    requires(Evaluator<Xpr> evaluator, std::size_t index, std::size_t row, std::size_t column)
    {
        typename Evaluator<Xpr>::XprType;
        typename Evaluator<Xpr>::CoeffType;
        { evaluator.Coefficient(index) } -> std::same_as<typename Evaluator<Xpr>::CoeffType>;
        { evaluator.Coefficient(row, column) } -> std::same_as<typename Evaluator<Xpr>::CoeffType>;
    } &&
    (
        // 对于左值表达式，求值器需要定义CoefficientRef接口
        !ContainFlag<Xpr, XprFlag::LeftValue>() ||
        requires(Evaluator<Xpr> evaluator, std::size_t index, std::size_t row, std::size_t column)
        {
            { evaluator.CoefficientRef(index) } -> std::same_as<typename Evaluator<Xpr>::CoeffType&>;
            { evaluator.CoefficientRef(row, column) } -> std::same_as<typename Evaluator<Xpr>::CoeffType&>;
        }
    );

    // 矩阵逐元素运算，要求矩阵元素类型相同以及行列相等
    template<typename LhsXpr, typename RhsXpr>
    concept CWiseOperable = IsMatrixExpression<LhsXpr> && IsMatrixExpression<RhsXpr> &&
            std::is_same_v<typename Traits<LhsXpr>::Scalar, typename Traits<RhsXpr>::Scalar> &&
            Traits<LhsXpr>::Rows == Traits<RhsXpr>::Rows && Traits<LhsXpr>::Columns == Traits<RhsXpr>::Columns;

    // 两个表达式是否可以执行矩阵乘法，左边表达式的列数要等于右边表达式的行数
    template<typename LhsXpr, typename RhsXpr>
    concept MatrixMultipliable = IsMatrixExpression<LhsXpr> && IsMatrixExpression<RhsXpr> &&
            std::is_same_v<typename Traits<LhsXpr>::Scalar, typename Traits<RhsXpr>::Scalar> &&
            Traits<LhsXpr>::Columns == Traits<RhsXpr>::Rows;

    // 表达式基类
    template<typename Derived> requires IsMatrixExpression<Derived>
    class MatrixBase;
	// 矩阵类
	template<typename Scalar, std::size_t Rows, std::size_t Cols, StorageOrder Order = DefaultOrder>
	class Matrix;
    // 二元表达式
    template<typename BinaryOp, typename LhsXpr, typename RhsXpr> requires CWiseOperable<LhsXpr, RhsXpr>
    class CWiseBinaryOp;
    // 二元运算
    template<typename Scalar> requires requires(Scalar a, Scalar b) { a + b; }
    struct ScalarSumOp;
    template<typename Scalar> requires requires(Scalar a, Scalar b) { a - b; }
    struct ScalarSubtractOp;
    template<typename Scalar> requires requires(Scalar a, Scalar b) { a * b; }
    struct ScalarProductOp;
    template<typename Scalar> requires requires(Scalar a, Scalar b) { a / b; }
    struct ScalarDivideOp;
    // 矩阵乘法表达式
    template<typename LhsXpr, typename RhsXpr> requires MatrixMultipliable<LhsXpr, RhsXpr>
    class Product;
    // 矩阵块表达式
    template<typename NestedXpr, std::size_t BlockRows, std::size_t BlockColumns>
    class Block;
    // 矩阵转置表达式
    template<typename NestedXpr>
    class Transpose;

    // 获取表达式对应的矩阵类型
    template<typename Xpr>
    struct PlainMatrixType
    {
        using Type = Matrix<typename Traits<Xpr>::Scalar, Traits<Xpr>::Rows, Traits<Xpr>::Columns,
                ContainFlag<Xpr, XprFlag::RowMajor>() ? StorageOrder::RowMajor : StorageOrder::ColumnMajor>;
    };
}

#include "MatrixBase.hpp"
#include "Matrix.hpp"
#include "Assignment.hpp"
#include "CWiseBinaryOp.hpp"
#include "Transpose.hpp"
#include "Block.hpp"
#include "Redux.hpp"
#include "Product.hpp"
#include "Dot.hpp"
