/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
	// 矩阵储存格式
	enum class StorageOption : char
	{
		RowMajor,
		ColMajor,
	};
	inline constexpr StorageOption DefaultMatrixStorageOrderOption = StorageOption::RowMajor;
	inline constexpr int Dynamic = -1;

	// 矩阵类
	template<typename ScalarT, int Rows, int Cols,
		StorageOption Options_ = (Rows == 1 && Cols != 1 ? StorageOption::RowMajor :
			Rows != 1 && Cols == 1 ? StorageOption::ColMajor :
			DefaultMatrixStorageOrderOption)
	> class Matrix;
}