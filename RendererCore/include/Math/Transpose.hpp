/********************************************************************
* TianGong RenderLab												*
* Copyright (c) Gaiyitp9. All rights reserved.						*
* This code is licensed under the MIT License (MIT).				*
*																	*
* Noted:															*
* This file is part of Eigen, a lightweight C++ template library	*
* for linear algebra which is subject to the terms of the			*
* Mozilla Public License v.2.0. And I made some simplifications.	*
*********************************************************************/
#pragma once

namespace LCH::Math
{

template<typename MatrixType>
struct traits<Transpose<MatrixType>>
{
private:
	using MatrixTypePlain = remove_all_t<MatrixType>;
	constexpr static Flag FlagsLvalueBit = is_lvalue<MatrixTypePlain> ? Flag::Lvalue : Flag::None;
	constexpr static Flag Flags0 = traits<MatrixTypePlain>::Flags & ~(Flag::Lvalue | Flag::NestByRef);	// 移除LvalueBit和NestByRefBit标志位
	constexpr static Flag Flags1 = Flags0 | FlagsLvalueBit;

public:
	using Scalar = MatrixTypePlain::Scalar;
	constexpr static int RowsAtCompileTime = MatrixTypePlain::ColsAtCompileTime;
	constexpr static int ColsAtCompileTime = MatrixTypePlain::RowsAtCompileTime;
	constexpr static int InnerStrideAtCompileTime = inner_stride_at_compile_time<MatrixTypePlain>::value;
	constexpr static int OuterStrideAtCompileTime = outer_stride_at_compile_time<MatrixTypePlain>::value;
	constexpr static Flag Flags = Flags1 ^ Flag::RowMajor;		// RowMajorBit位翻转											// RowMajorBit取反
};

template<typename MatrixType>
class Transpose : public MatrixBase<Transpose<MatrixType>>
{
public:
	using MatrixTypePlain = remove_all_t<MatrixType>;
	using MatrixTypeNested = ref_selector<MatrixTypePlain>::non_const_type;

	explicit Transpose(MatrixTypePlain& matrix) : m_matrix(matrix) {}

	constexpr int rows() const { return m_matrix.cols(); }
	constexpr int cols() const { return m_matrix.rows(); }
	const MatrixTypePlain& nestedExpression() const { return m_matrix; }
	MatrixTypePlain& nestedExpression() { return m_matrix; }

private:
	MatrixTypeNested m_matrix;
};

}