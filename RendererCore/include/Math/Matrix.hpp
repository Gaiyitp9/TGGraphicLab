/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{
	template<typename Scalar_, int Rows_, int Cols_, int Options_, int MaxRows_, int MaxCols_>
	class Matrix
		: public MatrixBase<Matrix<Scalar_, Rows_, Cols_, Options_, MaxRows_, MaxCols_>>
	{

	};
}