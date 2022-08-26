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
	public:
		using Scalar = traits<Derived>::Scalar;
		static constexpr int RowsAtCompileTime = traits<Derived>::RowsAtCompileTime;
		static constexpr int ColsAtCompileTime = traits<Derived>::ColsAtCompileTime;
		static constexpr int SizeAtCompileTime = size_at_compile_time(traits<Derived>::RowsAtCompileTime, traits<Derived>::ColsAtCompileTime);
		static constexpr bool IsVectorAtCompileTime = traits<Derived>::RowsAtCompileTime == 1 ||
													  traits<Derived>::ColsAtCompileTime == 1;
		static constexpr bool IsRowMajor = bool(traits<Derived>::Flags & RowMajorBit);

	public:
		Derived& derived() { return *static_cast<Derived*>(this); }
		const Derived& derived() const { return *static_cast<const Derived*>(this); }

	public:
		template<typename OtherDerived>
		CwiseBinaryOp<scalar_sum_op<Scalar, typename traits<OtherDerived>::Scalar>, 
			Derived, OtherDerived> operator+(const MatrixBase<OtherDerived>& other)
		{
			return CwiseBinaryOp<scalar_sum_op<Scalar, typename traits<OtherDerived>::Scalar>, 
				Derived, OtherDerived>(derived(), other.derived());
		}
	};
}