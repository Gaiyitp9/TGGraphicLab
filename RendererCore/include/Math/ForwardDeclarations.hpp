/************************************************************************
* This file is part of Eigen, a lightweight C++ template library		*
* for linear algebra.													*
*																		*
* Copyright (C) 2011 Benoit Jacob <jacob.benoit.1@gmail.com>			*
* Copyright (C) 2011-2014 Gael Guennebaud <gael.guennebaud@inria.fr>	*
* Copyright (C) 2011-2012 Jitse Niesen <jitse@maths.leeds.ac.uk>		*
*																		*
* This Source Code Form is subject to the terms of the Mozilla			*
* Public License v. 2.0. If a copy of the MPL was not distributed		*
* with this file, You can obtain one at http://mozilla.org/MPL/2.0/.	*
*																		*
* Noted: I made some modifications in this file.						*
*************************************************************************/
#pragma once

namespace LCH::Math
{

template<typename T> struct traits;
template<typename T> struct traits<const T> : traits<T> {};

// 是否可以直接访问底层数据
template<typename XprType>
inline constexpr bool has_direct_access = NotNone(traits<XprType>::Flags & Flag::DirectAccess);

// 是否可以写入数据
template<typename XprType>
inline constexpr bool has_write_access = NotNone(traits<XprType>::Flags & Flag::Lvalue);

// 访问级别
template<typename Derived> 
inline constexpr AccessorLevel accessors_level = has_direct_access<Derived> 
	? (has_write_access<Derived> ? AccessorLevel::DirectWrite : AccessorLevel::Direct)
	: (has_write_access<Derived> ? AccessorLevel::Write : AccessorLevel::ReadOnly);

// 最高层基类
template<typename Derived> struct Base;
// 矩阵系数访问类
template<typename Derived, AccessorLevel Level> class CoeffsBase;
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
template<typename XprType> class Transpose;
// 块
template<typename XprType, int BlockRows = Dynamic, int BlockCols = Dynamic> class Block;
// 映射基类
template<typename Derived, 
	AccessorLevel Level = has_write_access<Derived> ? AccessorLevel::Write : AccessorLevel::ReadOnly>
class MapBase;

// 二元运算
template<typename BinaryOp, typename Lhs, typename Rhs> class CwiseBinaryOp;
// 加法运算
template<typename Scalar> class ScalarSumOp;
// 减法运算
template<typename Scalar> class ScalarSubOp;
// 乘法运算
template<typename Scalar> class ScalarProductOp;

// 表达式求值器
template<typename T> class Evaluator;
template<typename T> class Evaluator<const T> : Evaluator<T> {};

}