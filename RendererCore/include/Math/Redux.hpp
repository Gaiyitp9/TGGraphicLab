/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{

template<typename Derived>
typename traits<Derived>::Scalar MatrixBase<Derived>::sum() const
{
	if (SizeAtCompileTime == 0 || (SizeAtCompileTime == Dynamic && size() == 0))
		return Scalar(0);

	return derived().redux(scalar_sum_op<Scalar, Scalar>());
}

template<typename Derived>
template<typename Functor>
typename traits<Derived>::Scalar MatrixBase<Derived>::redux(const Functor& func) const
{
	assert(rows() > 0 && cols() > 0 && "You are using an empty matrix");


}

}