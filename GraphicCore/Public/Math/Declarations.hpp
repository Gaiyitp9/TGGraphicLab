/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    // 给枚举添加逻辑运算
    template<typename Enum> requires std::is_enum_v<Enum>
    constexpr Enum operator|(Enum left, Enum right)
    {
        return static_cast<Enum>(std::to_underlying(left) | std::to_underlying(right));
    }

    template<typename Enum> requires std::is_enum_v<Enum>
    constexpr Enum operator^(Enum left, Enum right)
    {
        return static_cast<Enum>(std::to_underlying(left) ^ std::to_underlying(right));
    }

    template<typename Enum> requires std::is_enum_v<Enum>
    constexpr Enum operator&(Enum left, Enum right)
    {
        return static_cast<Enum>(std::to_underlying(left) & std::to_underlying(right));
    }

    template<typename Enum> requires std::is_enum_v<Enum>
    constexpr Enum operator~(Enum e)
    {
        return static_cast<Enum>(~std::to_underlying(e));
    }

    // 表达式特性，每种表达式都需要特化该类
    // 把表达式的特性定义在Traits里的原因：矩阵表达式基类使用CRTP(Curiously Recurring Template Pattern)，
    // 基类需要访问表达式的特性(比如Rows)，直接把特性定义在表达式中无法访问，因为在基类中的Derived是incomplete的
    // (我觉得这里是指派生类还未实例化，因为此时正在实例化基类)。可以使用Traits<Derived>来访问。
    // https://stackoverflow.com/questions/6006614/c-static-polymorphism-crtp-and-using-typedefs-from-derived-classes
    template<typename Xpr> struct Traits;
    template<typename Xpr> struct Traits<const Xpr> : Traits<Xpr> {};
    // 表达式求值器，每种表达式都需要特化自己的求值器
    // 分为两种类型的求值器: 1. 根据索引返回值，用于计算 2. 根据索引返回引用，用来修改表达式的值
    // 获取矩阵的单个元素或packet(类似一个向量，用于SIMD运算)
    template<typename Xpr, bool IsConst = true> class Evaluator;
    template<typename Xpr> using RefEvaluator = Evaluator<Xpr, false>;

    // 矩阵储存顺序
    enum class StorageOrder : unsigned char
    {
        RowMajor,
        ColumnMajor,
    };
    // 默认储存顺序，可以使用宏定义修改
#ifdef TG_ROW_MAJOR_MATRIX
    inline constexpr auto DefaultOrder = StorageOrder::RowMajor;
#else
    inline constexpr StorageOrder DefaultOrder = StorageOrder::ColumnMajor;
#endif

    // 矩阵表达式标志
    enum class XprFlag : unsigned char
    {
        None            = 0,
        RowMajor        = 1,        // 按行储存
        LeftValue       = 1 << 1,   // 可以作为左值
        LinearAccess    = 1 << 2,   // 以一维向量的方式访问，也就是求值器定义了Entry(index)函数
        NestByRef       = 1 << 3,   // 以引用形式嵌入其他表达式中
    };

    // 赋值遍历类型
    enum class Traversal : unsigned char
    {
        Default,
        Linear,
        Vectorized,
    };

    enum class ProductType : unsigned char
    {
        Default,
        Lazy,       // 矩阵乘法延迟求值(Lazy evaluation)
    };

    // 矩阵表达式支持的数据类型，用来限制自定义数据类型
    template<typename Scalar>
    concept IsMatrixScalar = requires(Scalar a, Scalar b)
    {
        { a + b } -> std::convertible_to<Scalar>;
        { a - b } -> std::convertible_to<Scalar>;
        { a * b } -> std::convertible_to<Scalar>;
        { a / b } -> std::convertible_to<Scalar>;
        a = b;
    };

    // 表达式标志是否包含指定标志
    template<typename Xpr, XprFlag flag>
    constexpr bool HasFlag = (Traits<Xpr>::Flags & flag) != XprFlag::None;

    // 矩阵表达式概念，分为两部分: 1. 矩阵表达式的特性 2. 表达式求值器的要求
    template<typename Xpr>
    concept IsMatrixExpression = requires
    {
        requires IsMatrixScalar<typename Traits<Xpr>::Scalar>;
        Traits<Xpr>::Rows;
        Traits<Xpr>::Columns;
        Traits<Xpr>::Size;
        Traits<Xpr>::Flags;
    } &&
    requires(Evaluator<Xpr> evaluator, std::size_t index, std::size_t row, std::size_t column)
    {
        requires std::constructible_from<Evaluator<Xpr>, const Xpr&>;
        { evaluator.Entry(row, column) } -> std::same_as<typename Traits<Xpr>::Scalar>;
        requires !HasFlag<Xpr, XprFlag::LinearAccess> ||
            requires { { evaluator.Entry(index) } -> std::same_as<typename Traits<Xpr>::Scalar>; };
    } &&
    (
        !HasFlag<Xpr, XprFlag::LeftValue> ||
        // 左值表达式求值器概念
        requires(RefEvaluator<Xpr> evaluator, std::size_t index, std::size_t row, std::size_t column)
        {
            requires std::constructible_from<RefEvaluator<Xpr>, Xpr&>;
            { evaluator.Entry(row, column) } -> std::same_as<typename Traits<Xpr>::Scalar&>;
            requires !HasFlag<Xpr, XprFlag::LinearAccess> ||
                requires { { evaluator.Entry(index) } -> std::same_as<typename Traits<Xpr>::Scalar&>; };
        }
    );

    // 表达式基类
    template<typename Derived> requires IsMatrixExpression<Derived>
    class MatrixBase;
	// 矩阵类
	template<typename Scalar, std::size_t Rows, std::size_t Cols, StorageOrder Order = DefaultOrder>
	class Matrix;
    // 二元表达式
    template<typename BinaryOp, typename LhsXpr, typename RhsXpr>
    class CWiseBinaryOp;
    // 二元运算
    template<typename Scalar> struct ScalarAddOp;
    template<typename Scalar> struct ScalarSubtractOp;
    template<typename Scalar> struct ScalarProductOp;
    template<typename Scalar> struct ScalarDivideOp;
    // 矩阵乘法表达式
    template<typename LhsXpr, typename RhsXpr, ProductType Type>
    class Product;
    // 矩阵块表达式
    template<typename Xpr, std::size_t BlockRows, std::size_t BlockColumns>
    class Block;
    // 矩阵转置表达式
    template<typename Xpr> class Transpose;
    // 零元运算，不接受任何矩阵表达式的运算符
    template<typename NullaryOp, typename Matrix> class CWiseNullaryOp;
    // 生成单位矩阵运算符
    template<typename Scalar> struct ScalarIdentityOp;
    // 生成常量矩阵运算符
    template<typename Scalar> struct ScalarConstantOp;
    // 逆矩阵表达式
    template<typename Xpr> class Inverse;

    // 表达式对应的矩阵类型
    template<typename Xpr>
    using PlainMatrix = Matrix<typename Traits<Xpr>::Scalar, Traits<Xpr>::Rows, Traits<Xpr>::Columns,
        HasFlag<Xpr, XprFlag::RowMajor> ? StorageOrder::RowMajor : StorageOrder::ColumnMajor>;

    // 表达式嵌入形式
    template<typename Xpr>
    struct RefSelector
    {
        using Type = std::conditional_t<HasFlag<Xpr, XprFlag::NestByRef>, const Xpr&, const Xpr>;
        using NonConstType = std::conditional_t<HasFlag<Xpr, XprFlag::NestByRef>, Xpr&, Xpr>;
    };

    // 旋转基类
    template<typename Derived, int Dimension>
    class RotationBase;
    // 旋转轴和旋转角表示的旋转
    template<typename Scalar>
    class AngleAxis;
    // 四元数表示的旋转
    template<typename Scalar>
    class Quaternion;
    // 物体变换
    template<typename Scalar, std::size_t Dimension, StorageOrder Order = DefaultOrder>
    class Transform;
}
