/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <type_traits>

namespace LCH::Math
{
	struct support_simd;
}

// 使用SIMD需要支持AVX2或AVX指令集
#if defined(__AVX2__) || defined(__AVX__)
#include <immintrin.h>
struct LCH::Math::support_simd : std::true_type { };
#else
struct LCH::Math::support_simd : std::false_type { };
#endif

namespace LCH::Math
{
	template<typename T>
	struct simd_trait;

	template<>
	struct simd_trait<float>
	{
		static constexpr size_t Alignment = 16;

		static void add(float const* lhs, float const* rhs, float* result)
		{
			__m128 val = _mm_add_ps(_mm_load_ps(lhs), _mm_load_ps(rhs));
			_mm_store_ps(result, val);
		}

		static void sub(float const* lhs, float const* rhs, float* result)
		{
			__m128 val = _mm_sub_ps(_mm_load_ps(lhs), _mm_load_ps(rhs));
			_mm_store_ps(result, val);
		}

		static void dot(float const* lhs, float const* rhs, float* result)
		{
			__m128 val = _mm_dp_ps(_mm_load_ps(lhs), _mm_load_ps(rhs), 31);
			_mm_store_ps(result, val);
		}
	};

	template<>
	struct simd_trait<int>
	{
		static constexpr size_t Alignment = 16;
	};

	template<>
	struct simd_trait<double>
	{
		static constexpr size_t Alignment = 32;
	};
}