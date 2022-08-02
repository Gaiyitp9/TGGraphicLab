/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{
	template<typename T>
	struct traits;

	template<typename T>
	struct traits<const T> : traits<T> {};

	template<typename Scalar_, int Rows, int Cols, 
		int Options_ = AutoAlign | (Rows == 1 && Cols != 1 ? StorageOption::RowMajor :
									Rows != 1 && Cols == 1 ? StorageOption::ColMajor :
									DEFAULT_MATRIX_STORAGE_ORDER_OPTION)
	>
	class Matrix;
}