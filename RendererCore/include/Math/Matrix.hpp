/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{
	template<typename Scalar_, int Rows, int Cols, int Options_>
	struct traits<Matrix<Scalar_, Rows, Cols, Options_>>
	{
	public:
		using Scalar = Scalar_;
		constexpr static int RowsAtCompileTime = Rows;
		constexpr static int ColsAtCompileTime = Cols;
		constexpr static int Options = Options_;
	};

	template<typename Scalar_, int Rows, int Cols, int Options_>
	class Matrix
		: public MatrixBase<Matrix<Scalar_, Rows, Cols, Options_>>
	{

	};
}