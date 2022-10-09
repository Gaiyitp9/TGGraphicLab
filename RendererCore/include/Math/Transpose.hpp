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
	constexpr static Flag Flags = Flags1 ^ Flag::RowMajor;		// RowMajorBit位翻转											// RowMajorBit取反
};

template<typename MatrixType>
class Transpose : public MatrixBase<Transpose<MatrixType>>
{
public:
	using Base = MatrixBase<Transpose<MatrixType>>;
	using MatrixTypePlain = remove_all_t<MatrixType>;
	using MatrixTypeNested = ref_selector<MatrixTypePlain>::non_const_type;
	using typename Base::Scalar;
	using ScalarWithConstIfNotLvalue = std::conditional_t<is_lvalue<MatrixType>, Scalar, const Scalar>;

	explicit Transpose(MatrixTypePlain& matrix) : m_matrix(matrix) {}

	constexpr int rows() const { return m_matrix.cols(); }
	constexpr int cols() const { return m_matrix.rows(); }
	ScalarWithConstIfNotLvalue* data() { return m_matrix.data(); }
	const Scalar* data() const { return m_matrix.data(); }
	const MatrixTypePlain& nestedExpression() const { return m_matrix; }
	MatrixTypePlain& nestedExpression() { return m_matrix; }

private:
	MatrixTypeNested m_matrix;
};

}