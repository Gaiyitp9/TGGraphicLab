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
		static constexpr bool IsVectorAtCompileTime = traits<Derived>::RowsAtCompileTime == 1 ||
													  traits<Derived>::ColsAtCompileTime == 1;

	public:

		MatrixBase operator+(const MatrixBase&);
	};
}