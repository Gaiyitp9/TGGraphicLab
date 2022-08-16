/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{
	// 矩阵和所有矩阵表达式的基类
	template<typename Derived>
	class MatrixBase
	{
		using Scalar = traits<Derived>::Scalar;
		using RowsAtCompileTime = traits<Derived>::RowsAtCompileTime;
		using ColsAtCompileTime = traits<Derived>::ColsAtCompileTime;
		using SizeAtCompileTime = traits<Derived>::SizeAtCompileTime;
		using Options = traits<Derived>::Options;
		using Alignment = traits<Derived>::Alignment;
		static constexpr bool IsVectorAtCompileTime = traits<Derived>::RowsAtCompileTime == 1 ||
													  traits<Derived>::ColsAtCompileTime == 1;

	public:
		Derived& derived() { return *static_cast<Derived*>(this); }
		const Derived& derived() { return *static_cast<const Derived*>(this); }

	public:
		template<typename OtherDerived>
		CwiseBinaryOp operator+(const MatrixBase<OtherDerived>& other)
		{
			return CwiseBinaryOp<scalar_sum_op<Scalar, traits<OtherDerived>::Scalar>, 
				Derived, OtherDerived>(derived(), other.derived());
		}
	};
}