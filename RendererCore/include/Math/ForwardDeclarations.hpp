/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{
	template<typename T> struct traits;
	template<typename T> struct traits<const T> : traits<T> {};

	// 是否可以直接访问底层数据
	template<typename Derived> 
	struct has_direct_access
	{
		constexpr static bool value = (traits<Derived>::Flags & DirectAccessBit) ? true : false;
	};

	template<typename Derived> 
	struct accessors_level
	{
		constexpr static bool has_direct_access = static_cast<bool>(traits<Derived>::Flags & DirectAccessBit);
	};

	// 矩阵及矩阵表达式基类
	template<typename Derived> class MatrixBase;
	// 矩阵类
	template<typename ScalarT, int Rows, int Cols, 
		StorageOption Options_ = (Rows == 1 && Cols != 1 ? StorageOption::RowMajor :
						Rows != 1 && Cols == 1 ? StorageOption::ColMajor :
						DefaultMatrixStorageOrderOption)
	> class Matrix;
	// 矩阵底层存储类
	template<typename T, int Size, int Rows, int Cols> class Storage;
	// 转置
	template<typename MatrixType> class Transpose;

	// 二元运算
	template<typename BinaryOp, typename Lhs, typename Rhs> class CwiseBinaryOp;
	// 加法运算
	template<typename LhsScalar, typename RhsScalar> struct scalar_sum_op;

	// 表达式求值器
	template<typename T> struct evaluator;
	template<typename T> struct evaluator<const T> : evaluator<T> {};
}