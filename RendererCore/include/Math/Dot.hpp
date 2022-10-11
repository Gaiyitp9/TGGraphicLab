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

template<typename Lhs, typename Rhs,
	bool NeedTranspose = (Lhs::RowsAtCompileTime == 1 && Rhs::ColsAtCompileTime) ||
							(Lhs::ColsAtCompileTime == 1 && Rhs::RowsAtCompileTime)>
struct dot_nocheck
{
	using product = scalar_product_op<typename traits<Lhs>::Scalar, typename traits<Rhs>::Scalar>;
	using result_type = product::result_type;

	static result_type Run(const Lhs& a, const Rhs& b)
	{
		return a.template BinaryExpr<product>(b).sum();
	}
};

template<typename Lhs, typename Rhs>
struct dot_nocheck<Lhs, Rhs, true>
{
	using product = scalar_product_op<typename traits<Lhs>::Scalar, typename traits<Rhs>::Scalar>;
	using result_type = product::result_type;

	static result_type Run(const Lhs& a, const Rhs& b)
	{
		return a.transpose().template BinaryExpr<product>(b).sum();
	}
};

template<typename Derived>
template<typename OtherDerived>
typename scalar_binaryop_traits<typename traits<Derived>::Scalar, typename traits<OtherDerived>::Scalar>::return_type
MatrixBase<Derived>::Dot(const MatrixBase<OtherDerived>& other) const
{
	static_assert(Derived::IsVectorAtCompileTime, "You are calling a vector method on matrix.");
	static_assert(OtherDerived::IsVectorAtCompileTime, "You are calling a vector method on matrix.");
	static_assert(have_same_vector_size<Derived, OtherDerived>, "You mix vectors of different sizes.");
	static_assert(has_return_type<scalar_binaryop_traits<Scalar, typename OtherDerived::Scalar>>::value, "You mix different numeric types.");
	assert(size() == other.size());

	return dot_nocheck<Derived, OtherDerived>::Run(derived(), other.derived());
}

}