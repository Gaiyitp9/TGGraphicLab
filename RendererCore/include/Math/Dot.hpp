/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
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
		return a.template binaryExpr<product>(b).sum();
	}
};

template<typename Lhs, typename Rhs>
struct dot_nocheck<Lhs, Rhs, true>
{
	using product = scalar_product_op<typename traits<Lhs>::Scalar, typename traits<Rhs>::Scalar>;
	using result_type = product::result_type;

	static result_type Run(const Lhs& a, const Rhs& b)
	{
		return a.transpose().template binaryExpr<product>(b).sum();
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

	return dot_nocheck<Derived, OtherDerived>::Run(*this, other);
}

}