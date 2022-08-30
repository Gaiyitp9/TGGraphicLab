/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{

enum class StorageOption : char
{
	RowMajor = 0x1,
	ColMajor = 0x2,
};
const StorageOption DefaultMatrixStorageOrderOption = StorageOption::RowMajor;
const int Dynamic = -1;

enum Flags : unsigned int
{
	RowMajorBit = 0x1,
	EvalBeforeNestingBit = 0x2,
	PacketAccessBit = 0x4,
	LinearAccessBit = 0x10,
	LvalueBit = 0x20,
	DirectAccessBit = 0x40,
	NestByRefBit = 0x100,
};

enum AccessorLevels : char
{
	ReadOnlyAccessors,
	WriteAccessors,
	DirectAccessors,
	DirectWriteAccessors,
};

enum TranversalType : char
{
	DefaultTraversal,
	LinearTraversal,
	InnerVectorizedTraversal,
	LinearVectorizedTraversal,
	InvalidTraversal,
	AllAtOnceTraversal,
};

enum UnrollingType : char
{
	NoUnrolling,
	CompleteUnrolling,
};

}