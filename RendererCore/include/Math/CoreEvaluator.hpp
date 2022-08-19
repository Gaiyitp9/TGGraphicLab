/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{

// 二元运算求值
template<typename T, 
		 typename LhsScalar = typename traits<typename T::Lhs>::Scalar,
		 typename RhsScalar = typename traits<typename T::Rhs>::Scalar>
struct binary_evaluator;
}