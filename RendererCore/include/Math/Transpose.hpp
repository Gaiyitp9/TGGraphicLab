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

template<typename XprType>
struct traits<Transpose<XprType>>
{
private:
	using XprTypePlain = std::remove_reference_t<XprType>;
	constexpr static Flag FlagsLvalueBit = is_lvalue<XprTypePlain> ? Flag::Lvalue : Flag::None;
	constexpr static Flag Flags0 = traits<XprTypePlain>::Flags & ~(Flag::Lvalue | Flag::NestByRef);	// 移除LvalueBit和NestByRefBit标志位
	constexpr static Flag Flags1 = Flags0 | FlagsLvalueBit;

public:
	using Scalar = XprType::Scalar;
	constexpr static int RowsAtCompileTime = XprType::ColsAtCompileTime;
	constexpr static int ColsAtCompileTime = XprType::RowsAtCompileTime;
	constexpr static Flag Flags = Flags1 ^ Flag::RowMajor;		// RowMajorBit位翻转
	constexpr static int InnerStrideAtCompileTime = inner_stride_at_compile_time<XprTypePlain>;
	constexpr static int OuterStrideAtCompileTime = outer_stride_at_compile_time<XprTypePlain>;
	constexpr static int Alignment = traits<XprTypePlain>::Alignment;
};

template<typename XprType, bool HasDirectAccess = has_direct_access<XprType>>
class TransposeImpl : public MatrixBase<Transpose<XprType>>
{

};

template<typename XprType>
class TransposeImpl<XprType, true> : public MatrixBase<Transpose<XprType>>
{
public:
	using Base = MatrixBase<Transpose<XprType>>;
	using typename Base::Scalar;
	using ScalarWithConstIfNotLvalue = std::conditional_t<is_lvalue<XprType>, Scalar, const Scalar>;
	using Base::derived;

	int innerStride() { return derived().nestedExpression().innerStride(); }
	int outerStride() { return derived().nestedExpression().outerStride(); }
	ScalarWithConstIfNotLvalue* data() { return derived().nestedExpression().data(); }
	const Scalar* data() const { return derived().nestedExpression().data(); }
};

template<typename XprType>
class Transpose : public TransposeImpl<XprType>
{
public:
	using XprTypePlain = remove_all_t<XprType>;
	using XprTypeNested = ref_selector<XprTypePlain>::non_const_type;
	
	explicit Transpose(XprTypePlain& matrix) : m_matrix(matrix) {}

	constexpr int rows() const { return m_matrix.cols(); }
	constexpr int cols() const { return m_matrix.rows(); }
	const XprTypePlain& nestedExpression() const { return m_matrix; }
	XprTypePlain& nestedExpression() { return m_matrix; }

private:
	XprTypeNested m_matrix;
};

}