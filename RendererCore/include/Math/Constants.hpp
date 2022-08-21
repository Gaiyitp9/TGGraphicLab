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
		RowMajor = 0x1,
		ColMajor = 0x2,
	};
	const StorageOption DefaultMatrixStorageOrderOption = StorageOption::RowMajor;
	const int Dynamic = -1;

	const unsigned int RowMajorBit = 0x1;
	const unsigned int EvalBeforeNestingBit = 0x2;
	const unsigned int PacketAccessBit = 0x4;
	const unsigned int LinearAccessBit = 0x10;
	const unsigned int LvalueBit = 0x20;
	const unsigned int DirectAccessBit = 0x40;
	const int NestByRefBit = 0x100;
}