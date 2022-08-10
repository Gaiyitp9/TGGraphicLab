/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <array>
#include <type_traits>

namespace LCH::Math
{
	struct instruction_sse {};
	struct instruction_avx {};

	// 根据向量和矩阵的维度选择指令集
	template<typename T, size_t Dimension, typename Enable = void>
	struct SimdInstruction
	{
		constexpr static int Alignment = 16;
		using type = instruction_sse;
	};
	// 偏特化
	template<typename T, size_t Dimension>
	struct SimdInstruction<T, Dimension, std::enable_if_t<std::is_same_v<T, double> || (Dimension >= 16)>>
	{ 
		constexpr static int Alignment = 32;
		using type = instruction_avx;
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
	// 是否支持SIMD的trait
	struct support_simd : std::true_type { };

	template<typename T, typename InstructionSet>
	struct simd_trait;

	template<>
	struct simd_trait<float, instruction_sse>
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

		static void elementwise_product(float const* lhs, float const* rhs, float* result)
		{
			__m128 val = _mm_mul_ps(_mm_load_ps(lhs), _mm_load_ps(rhs));
			_mm_store_ps(result, val);
		}

		static void elementwise_div(float const* lhs, float const* rhs, float* result)
		{
			__m128 val = _mm_div_ps(_mm_load_ps(lhs), _mm_load_ps(rhs));
			_mm_store_ps(result, val);
		}

		static float dot(float const* lhs, float const* rhs)
		{
			__m128 dot = _mm_dp_ps(_mm_load_ps(lhs), _mm_load_ps(rhs), 0xF1);
			return _mm_cvtss_f32(dot);
		}
	};

	template<>
	struct simd_trait<float, instruction_avx>
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

		static void elementwise_product(float const* lhs, float const* rhs, float* result)
		{
			__m256 val = _mm256_mul_ps(_mm256_load_ps(lhs), _mm256_load_ps(rhs));
			_mm256_store_ps(result, val);
		}

		static void elementwise_div(float const* lhs, float const* rhs, float* result)
		{
			__m256 val = _mm256_div_ps(_mm256_load_ps(lhs), _mm256_load_ps(rhs));
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
	struct simd_trait<int, instruction_sse>
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

		static void elementwise_product(int const* lhs, int const* rhs, int* result)
		{
			__m128i val = _mm_mullo_epi32(_mm_load_si128((__m128i*)lhs), _mm_load_si128((__m128i*)rhs));
			_mm_store_si128((__m128i*)result, val);
		}

		static void elementwise_div(int const* lhs, int const* rhs, int* result)
		{
			__m128i val = _mm_div_epi32(_mm_load_si128((__m128i*)lhs), _mm_load_si128((__m128i*)rhs));
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
	struct simd_trait<int, instruction_avx>
	{
		static constexpr size_t Alignment = 32;
		static constexpr size_t DataCount = 8;

		static void add(int const* lhs, int const* rhs, int* result)
		{
			__m256i val = _mm256_add_epi32(_mm256_load_si256((__m256i*)lhs), _mm256_load_si256((__m256i*)rhs));
			_mm256_store_si256((__m256i*)result, val);
		}

		static void sub(int const* lhs, int const* rhs, int* result)
		{
			__m256i val = _mm256_sub_epi32(_mm256_load_si256((__m256i*)lhs), _mm256_load_si256((__m256i*)rhs));
			_mm256_store_si256((__m256i*)result, val);
		}

		static void elementwise_product(int const* lhs, int const* rhs, int* result)
		{
			__m256i val = _mm256_mullo_epi32(_mm256_load_si256((__m256i*)lhs), _mm256_load_si256((__m256i*)rhs));
			_mm256_store_si256((__m256i*)result, val);
		}

		static void elementwise_div(int const* lhs, int const* rhs, int* result)
		{
			__m256i val = _mm256_div_epi32(_mm256_load_si256((__m256i*)lhs), _mm256_load_si256((__m256i*)rhs));
			_mm256_store_si256((__m256i*)result, val);
		}

		static int dot(int const* lhs, int const* rhs)
		{
			__m256i val = _mm256_mullo_epi32(_mm256_load_si256((__m256i*)lhs), _mm256_load_si256((__m256i*)rhs));
			val = _mm256_hadd_epi32(val, val);
			val = _mm256_hadd_epi32(val, val);
			__m128i dot = _mm_add_epi32(_mm256_extractf128_si256(val, 1), _mm256_castsi256_si128(val));
			return _mm_cvtsi128_si32(dot);
		}
	};

	template<>
	struct simd_trait<double, instruction_avx>
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

		static void elementwise_product(double const* lhs, double const* rhs, double* result)
		{
			__m256d val = _mm256_mul_pd(_mm256_load_pd(lhs), _mm256_load_pd(rhs));
			_mm256_store_pd(result, val);
		}

		static void elementwise_div(double const* lhs, double const* rhs, double* result)
		{
			__m256d val = _mm256_div_pd(_mm256_load_pd(lhs), _mm256_load_pd(rhs));
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