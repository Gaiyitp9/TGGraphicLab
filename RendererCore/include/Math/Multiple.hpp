/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
	template<typename Matrix, int Row, int Col, int Rows, int Cols>
	struct DefaultMultiple
	{

	};

	template<typename Matrix, int Rows, int Cols>
	struct DefaultMultiple<Matrix, Rows, Cols, Rows, Cols>
	{

	};

	template<typename Matrix>
	struct Multiple
	{
		static void Run()
		{

		}
	};
}