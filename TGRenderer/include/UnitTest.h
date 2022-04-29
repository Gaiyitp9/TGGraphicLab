/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Chronometer.h"
#include "Diagnostics/MemoryDbg.h"
#include <ctime>
#include <string>

namespace LCH
{
	class UnitTest
	{
	public:
		void FormatTest();
		void TextEncodeTest();
		void TimeTest();
		void ArrayAlignmentTest();
		void MathLibTest();
		void SIMDTest();

	private:
		void NormalAdd(int* nums, size_t n);
		void SSEAdd(int* nums, size_t n);
		void AVX2Add(int* nums, size_t n);

	private:
		Chronometer timer;
	};
}