/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Diagnostics/MemoryDbg.h"
#include <array>
#include <type_traits>

namespace LCH::Math
{
	// 是否支持SIMD的trait
	struct support_simd;

	enum class InstructionType
	{
		SSE,
		AVX,
	};

	// 向量和矩阵的元素类型和维度的concept
	template<typename T, size_t Size>
	concept mathlib_type_and_size = (std::is_same_v<T, int> || std::is_same_v<T, float> ||
		std::is_same_v<T, double>) && (Size > 1);

	// 根据向量和矩阵的维度选择指令集
	template<size_t Size, typename Enable = void>
	struct SimdInstruction;
	// 根据维度定义两种特化
	template<size_t Size>
	struct SimdInstruction<Size, std::enable_if_t<(Size >= 16)>> 
	{ 
		static constexpr InstructionType type = InstructionType::AVX; 
	};

	template<size_t Size>
	struct SimdInstruction<Size, std::enable_if_t<(Size < 16)>>
	{ 
		static constexpr InstructionType type = InstructionType::SSE; 
	};

	// 内存对齐数组
	template<typename T, size_t Size, size_t Alignment = 16>
	class alignas(Alignment) aligned_array : public std::array<T, Size> {};
}

// 使用SIMD需要支持AVX2或AVX指令集
#if defined(__AVX2__) || defined(__AVX__)
#include <immintrin.h>

namespace LCH::Math
{
	struct support_simd : std::true_type { };

	template<typename T, InstructionType type>
	struct simd_trait;

	template<>
	struct simd_trait<float, InstructionType::SSE>
	{
		static constexpr size_t Alignment = 16;
		static constexpr size_t DataCount = 4;

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

		static float dot(float const* lhs, float const* rhs)
		{
			__m128 dot = _mm_dp_ps(_mm_load_ps(lhs), _mm_load_ps(rhs), 0xF1);
			return _mm_cvtss_f32(dot);
		}
	};

	template<>
	struct simd_trait<float, InstructionType::AVX>
	{
		static constexpr size_t Alignment = 32;
		static constexpr size_t DataCount = 8;

		static void add(float const* lhs, float const* rhs, float* result)
		{
			__m256 val = _mm256_add_ps(_mm256_load_ps(lhs), _mm256_load_ps(rhs));
			_mm256_store_ps(result, val);
		}

		static void sub(float const* lhs, float const* rhs, float* result)
		{
			__m256 val = _mm256_sub_ps(_mm256_load_ps(lhs), _mm256_load_ps(rhs));
			_mm256_store_ps(result, val);
		}

		static float dot(float const* lhs, float const* rhs)
		{
			__m256 val = _mm256_dp_ps(_mm256_load_ps(lhs), _mm256_load_ps(rhs), 0xF1);
			__m128 dot = _mm_add_ps(_mm256_extractf128_ps(val, 1), _mm256_castps256_ps128(val));
			return _mm_cvtss_f32(dot);
		}
	};

	template<>
	struct simd_trait<int, InstructionType::SSE>
	{
		static constexpr size_t Alignment = 16;
		static constexpr size_t DataCount = 4;

		static void add(int const* lhs, int const* rhs, int* result)
		{
			__m128i val = _mm_add_epi32(_mm_load_si128((__m128i*)lhs), _mm_load_si128((__m128i*)rhs));
			_mm_store_si128((__m128i*)result, val);
		}

		static void sub(int const* lhs, int const* rhs, int* result)
		{
			__m128i val = _mm_sub_epi32(_mm_load_si128((__m128i*)lhs), _mm_load_si128((__m128i*)rhs));
			_mm_store_si128((__m128i*)result, val);
		}

		static int dot(int const* lhs, int const* rhs)
		{
			__m128i dot = _mm_mullo_epi32(_mm_load_si128((__m128i*)lhs), _mm_load_si128((__m128i*)rhs));
			dot = _mm_hadd_epi32(dot, dot);
			dot = _mm_hadd_epi32(dot, dot);
			return _mm_cvtsi128_si32(dot);
		}
	};

	template<>
	struct simd_trait<double, InstructionType::AVX>
	{
		static constexpr size_t Alignment = 32;
		static constexpr size_t DataCount = 4;

		static void add(double const* lhs, double const* rhs, double* result)
		{
			__m256d val = _mm256_add_pd(_mm256_load_pd(lhs), _mm256_load_pd(rhs));
			_mm256_store_pd(result, val);
		}

		static void sub(double const* lhs, double const* rhs, double* result)
		{
			__m256d val = _mm256_sub_pd(_mm256_load_pd(lhs), _mm256_load_pd(rhs));
			_mm256_store_pd(result, val);
		}

		static double dot(double const* lhs, double const* rhs)
		{
			__m256d val = _mm256_mul_pd(_mm256_load_pd(lhs), _mm256_load_pd(rhs));
			val = _mm256_hadd_pd(val, val);
			__m128d dot = _mm_add_pd(_mm256_extractf128_pd(val, 1), _mm256_castpd256_pd128(val));
			return _mm_cvtsd_f64(dot);
		}
	};

#else
namespace LCH::Math
{
	struct support_simd : std::false_type { };
#endif

	constexpr bool support_simd_t = support_simd::value;
}