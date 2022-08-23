/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{

template<typename LhsScalar, typename RhsScalar>
struct scalar_sum_op
{
	using result_type = scalar_binaryop_traits<LhsScalar, RhsScalar, scalar_sum_op>::return_type;

	result_type operator()(const LhsScalar& a, const RhsScalar& b) const
	{
		return a + b;
	}
};

template<> bool scalar_sum_op<bool, bool>::operator()(const bool& a, const bool& b) const { return a || b; };

}