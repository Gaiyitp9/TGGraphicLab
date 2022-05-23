/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Simd.hpp"

namespace LCH::Math
{
	class Color
	{
		using simd = simd_trait<float, SimdInstruction<float, 4>::type>;
	public:
		Color();

		const float& r() const { return elements[0]; }
		float g() { return elements[1]; }
		float b() { return elements[2]; }
		float a() { return elements[3]; }



	private:
		aligned_array<float, 4, simd::Alignment> elements;
	};
}