/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{
	// 类特性
	template<typename T> struct traits;
	template<typename T> struct traits<const T> : traits<T> {};

	// 矩阵及矩阵表达式基类
	template<typename Derived> class MatrixBase;
	// 矩阵类
	template<typename ScalarT, int Rows, int Cols, 
		int Options_ = (Rows == 1 && Cols != 1 ? StorageOption::RowMajor :
						Rows != 1 && Cols == 1 ? StorageOption::ColMajor :
						DefaultMatrixStorageOrderOption)
	> class Matrix;
	// 矩阵底层存储类
	template<typename T, int Size, int Rows, int Cols, int Options> class Storage;

	// 二元运算
	template<typename BinaryOp, typename Lhs, typename Rhs> class CwiseBinaryOp;
	// 加法运算
	template<typename LhsScalar, typename RhsScalar> struct scalar_sum_op;

	// 表达式求值器
	template<typename T> struct evaluator;
	template<typename T> struct evaluator<const T> : evaluator<T> {};
}