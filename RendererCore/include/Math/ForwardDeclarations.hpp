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

	template<typename Derived> class MatrixBase;
	template<typename ScalarT, int Rows, int Cols, 
		int Options_ = (Rows == 1 && Cols != 1 ? StorageOption::RowMajor :
						Rows != 1 && Cols == 1 ? StorageOption::ColMajor :
						DefaultMatrixStorageOrderOption)
	> class Matrix;

	template<typename T, int Size, int Rows, int Cols, int Options> class Storage;

	template<typename BinaryOp, typename Lhs, typename Rhs> class CwiseBinaryOp;
	template<typename LhsScalar, typename RhsScalar> struct scalar_sum_op;
}