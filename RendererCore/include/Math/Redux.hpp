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

template<typename Derived>
typename MatrixBase<Derived>::Scalar MatrixBase<Derived>::sum() const
{
	if (SizeAtCompileTime == 0 || (SizeAtCompileTime == Dynamic && size() == 0))
		return Scalar(0);

	return derived().redux(scalar_sum_op<Scalar, Scalar>());
}

template<typename Derived>
template<typename Functor>
typename MatrixBase<Derived>::Scalar MatrixBase<Derived>::redux(const Functor& func) const
{
	assert(rows() > 0 && cols() > 0 && "You are using an empty matrix");


}

}