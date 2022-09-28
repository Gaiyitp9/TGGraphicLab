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

// 矩阵和所有矩阵表达式的基类
template<typename Derived>
class MatrixBase : public CoeffsBase<Derived, accessors_level<Derived>::value>
{
public:
	using Base = CoeffsBase<Derived, accessors_level<Derived>::value>;
	using typename Base::Scalar;
	using Base::derived;
	using Base::rows;
	using Base::cols;
	using Base::size;

	static constexpr int RowsAtCompileTime = traits<Derived>::RowsAtCompileTime;
	static constexpr int ColsAtCompileTime = traits<Derived>::ColsAtCompileTime;
	static constexpr int SizeAtCompileTime = size_at_compile_time(traits<Derived>::RowsAtCompileTime, traits<Derived>::ColsAtCompileTime);
	static constexpr bool IsVectorAtCompileTime = traits<Derived>::RowsAtCompileTime == 1 ||
												  traits<Derived>::ColsAtCompileTime == 1;
	static constexpr Flag Flags = traits<Derived>::Flags;
	static constexpr bool IsRowMajor = not_none(traits<Derived>::Flags & Flag::RowMajor);
	static constexpr int InnerSizeAtCompileTime = IsVectorAtCompileTime ? SizeAtCompileTime
												: IsRowMajor ? ColsAtCompileTime : RowsAtCompileTime;
	static constexpr int InnerStrideAtCompileTime = inner_stride_at_compile_time<Derived>::value;
	static constexpr int OuterStrideAtCompileTime = outer_stride_at_compile_time<Derived>::value;

public:
	constexpr int outerSize() const
	{
		return IsVectorAtCompileTime ? 1 : IsRowMajor ? rows() : cols();
	}
	constexpr int innerSize() const
	{
		return IsVectorAtCompileTime ? size() : IsRowMajor ? cols() : rows();
	}

public:
	template<typename OtherDerived>
	CwiseBinaryOp<scalar_sum_op<Scalar, typename traits<OtherDerived>::Scalar>, 
		Derived, OtherDerived> operator+(const MatrixBase<OtherDerived>& other)
	{
		return CwiseBinaryOp<scalar_sum_op<Scalar, typename traits<OtherDerived>::Scalar>, 
			Derived, OtherDerived>(derived(), other.derived());
	}

	template<typename OtherDerived>
	CwiseBinaryOp<scalar_sub_op<Scalar, typename traits<OtherDerived>::Scalar>,
		Derived, OtherDerived> operator-(const MatrixBase<OtherDerived>& other)
	{
		return CwiseBinaryOp<scalar_sub_op<Scalar, typename traits<OtherDerived>::Scalar>,
			Derived, OtherDerived>(derived(), other.derived());
	}

	Transpose<Derived> transpose()
	{
		return Transpose<Derived>(derived());
	}

	template<typename BinaryOp, typename OtherDerived>
	CwiseBinaryOp<BinaryOp, Derived, OtherDerived> binaryExpr(const MatrixBase<OtherDerived>& other)
	{
		return CwiseBinaryOp<BinaryOp, Derived, OtherDerived>(derived(), other.derived());
	}

	Scalar sum() const;

	template<typename BinaryOp>
	Scalar redux(const BinaryOp& func) const;

	template<typename OtherDerived>
	typename scalar_binaryop_traits<typename traits<Derived>::Scalar, typename traits<OtherDerived>::Scalar>::return_type
	Dot(const MatrixBase<OtherDerived>& other) const;
};

}