/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <type_traits>

namespace LCH::Math
{
	// 是否支持SIMD的trait
	struct support_simd;

	// 内存对齐数组
	template<typename T, size_t Size, size_t Alignment = 16>
	class alignas(Alignment) aligned_array : public std::array<T, Size> {};

	// 向量和矩阵的元素类型和维度的concept
	template<typename T, size_t Size>
	concept type_and_size = (std::is_same_v<T, int> || std::is_same_v<T, float> ||
		std::is_same_v<T, double>) && (Size > 1);
}

// 使用SIMD需要支持AVX2或AVX指令集
#if defined(__AVX2__) || defined(__AVX__)
#include <immintrin.h>

namespace LCH::Math
{
	struct support_simd : std::true_type { };

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

#else
namespace LCH::Math
{
	struct support_simd : std::false_type { };
#endif

	constexpr bool support_simd_t = support_simd::value;
}