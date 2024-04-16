/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    // 表达式特性，每种表达式都需要特化该类
	template<typename T> struct Traits;

    // 矩阵表达式标志
    enum class XprFlag : char
    {
        None            = 0,
        RowMajor        = 1,            // 按行储存
        DirectAccess    = 1 << 1,       // 是否能直接访问表达式的数据，也就是求值器有CoefficientRef函数
        LinearAccess    = 1 << 2,       // 是否能看作一维向量，也就是求值器可以调用Coefficient(int)函数
        Vector          = 1 << 3,       // 表达式是向量
        Square          = 1 << 4,       // 表达式是方阵
    };

	// 矩阵储存格式
	enum class StorageOption : char
	{
		RowMajor,
		ColumnMajor,
	};

#ifdef TG_ROW_MAJOR_MATRIX
	inline constexpr StorageOption DefaultMatrixStorageOrderOption = StorageOption::RowMajor;
#else
	inline constexpr StorageOption DefaultMatrixStorageOrderOption = StorageOption::ColumnMajor;
#endif

    // 矩阵表达式概念
    template<typename Xpr>
    concept MatrixExpression = requires
    {
        typename Traits<Xpr>::Scalar;
        Traits<Xpr>::Rows;
        Traits<Xpr>::Columns;
        Traits<Xpr>::Size;
        Traits<Xpr>::Flags;
    };

    // 矩阵逐元素运算，要求矩阵元素类型相同以及行列相等，如果是向量，则要求尺寸相同
    template<typename LhsXpr, typename RhsXpr>
    concept CWiseOperable = std::is_same_v<typename Traits<LhsXpr>::Scalar, typename Traits<RhsXpr>::Scalar> &&
            ((Traits<LhsXpr>::Flags & Traits<RhsXpr>::Flags & XprFlag::Vector) != XprFlag::None ?
                Traits<LhsXpr>::Size == Traits<RhsXpr>::Size :
                Traits<LhsXpr>::Rows == Traits<RhsXpr>::Rows && Traits<LhsXpr>::Columns == Traits<RhsXpr>::Columns);

    // 两个表达式是否可以执行矩阵乘法
    template<typename LhsXpr, typename RhsXpr>
    concept MatrixMultipliable = std::is_same_v<typename Traits<LhsXpr>::Scalar, typename Traits<RhsXpr>::Scalar> &&
                                 Traits<LhsXpr>::Columns == Traits<RhsXpr>::Rows;

    // 求值器概念
    // TODO: 后面再看看需不需要添加这个概念，跟MatrixExpression概念类似，求值器必须实现概念里的typedef和接口，
    //  但是需要额外添加一个基类，使用CRTP(奇异递归模板)技巧来添加概念，有点麻烦
    template<typename Evaluator>
    concept ExpressionEvaluator = requires(Evaluator evaluator)
    {
        typename Traits<Evaluator>::XprType;
        typename Traits<Evaluator>::CoeffType;
        { evaluator.Coefficient(0) } -> std::same_as<typename Evaluator::CoeffType>;
        { evaluator.Coefficient(0, 0) } -> std::same_as<typename Evaluator::CoeffType>;
    };

    // 表达式基类
    template<typename Derived> requires MatrixExpression<Derived> class MatrixBase;
	// 矩阵类
	template<typename Scalar_, int Rows_, int Cols_, StorageOption Option = DefaultMatrixStorageOrderOption>
	class Matrix;
    // 二元表达式
    template<typename BinaryOp, typename LhsXpr, typename RhsXpr> requires CWiseOperable<LhsXpr, RhsXpr>
    class CWiseBinaryOp;
    // 矩阵乘法表达式
    template<typename LhsXpr, typename RhsXpr> requires MatrixMultipliable<LhsXpr, RhsXpr>
    class Product;
    // 矩阵块表达式
    template<typename NestedXpr, int BlockRows, int BlockColumns>
    class Block;

    // 表达式求值器，每种表达式都需要特化该类
    template<typename Xpr> class Evaluator;

    // 加法函数
    template<typename Scalar> struct ScalarSumOp;
    // 减法函数
    template<typename Scalar> struct ScalarSubtractOp;
    // 乘法函数
    template<typename Scalar> struct ScalarProductOp;
    // 除法函数
    template<typename Scalar> struct ScalarDivideOp;
}
