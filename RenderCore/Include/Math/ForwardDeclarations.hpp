/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <cmath>

namespace TG::Math
{
    // 表达式特性，每种表达式都需要特化该类
	template<typename T> struct Traits;

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

    // 矩阵表达式标志
    enum class XprFlag
    {
        None        = 0,
        RowMajor    = 1,            // 按行储存
        Vector      = 1 << 2,       // 表达式是向量
        Square      = 1 << 3,       // 表达式是方阵
    };

    // 表达式基类
    template<typename Derived> class MatrixBase;
	// 矩阵类
	template<typename Scalar_, int Rows_, int Cols_, StorageOption Option = DefaultMatrixStorageOrderOption>
	class Matrix;
    // 二元表达式
    template<typename BinaryOp, typename LhsXpr, typename RhsXpr> class CWiseBinaryOp;

    // 表达式求值器，每种表达式都需要特化该类
    template<typename Xpr> class Evaluator;

    // 赋值运算
    template<typename Scalar> struct AssignOp;
    // 逐元素加法
    template<typename Scalar> struct ScalarSumOp;
    // 逐元素减法
    template<typename Scalar> struct ScalarSubtractOp;
    // 逐元素乘法
    template<typename Scalar> struct ScalarProductOp;
    // 逐元素除法
    template<typename Scalar> struct ScalarDivideOp;
}