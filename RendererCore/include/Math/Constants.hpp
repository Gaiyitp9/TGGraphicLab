/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{
	enum StorageOption
	{
		RowMajor = 0x01,
		ColMajor = 0x02,
		AutoAlign = 0x04,
		DontAlign = 0x08
	};

	const StorageOption DEFAULT_MATRIX_STORAGE_ORDER_OPTION = StorageOption::RowMajor;
}