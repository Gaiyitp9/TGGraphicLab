/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
	// 变量对应的包特性(包中有多个变量，表示SIMD中使用的变量，比如__m128)
	template<typename T>
	struct packet_traits
	{
		using type = T;
		using half = T;
		constexpr static int  Size = 1;						// 包尺寸
	};
	template<typename T> struct packet_traits<const T> : packet_traits<T> {};
	// 包的特性(通过包获得特性，上面是通过变量获得)
	template<typename T> struct unpacket_traits
	{
		using type = T;
		using half = T;
		constexpr static int Size = 1;
		constexpr static int Alignment = 1;
	};
	template<typename T> struct unpacket_traits<const T> : unpacket_traits<T> { };
}

// 使用SIMD需要支持AVX2或AVX指令集
#if defined(__AVX2__) || defined(__AVX__)

#include <immintrin.h>

namespace TG::Math
{
	inline constexpr int MIN_ALIGN_BYTES = 16;
	inline constexpr int MAX_ALIGN_BYTES = 32;
	inline constexpr int DEFAULT_ALIGN_BYTES = MAX_ALIGN_BYTES;
	inline constexpr bool Support_SIMD = true;

	using Packet4f = __m128;
	using Packet2d = __m128d;
	using Packet4i = __m128i;
	using Packet8f = __m256;
	using Packet4d = __m256d;
	using Packet8i = __m256i;

	template<>
	struct packet_traits<float>
	{
		using type = Packet8f;
		using half = Packet4f;
		constexpr static int  Size = 8;
	};

	template<>
	struct packet_traits<double>
	{
		using type = Packet4d;
		using half = Packet2d;
		constexpr static int  Size = 4;
	};

	template<>
	struct packet_traits<int>
	{
		using type = Packet8i;
		using half = Packet4i;
		constexpr static int  Size = 8;
	};

	template<>
	struct unpacket_traits<Packet8f>
	{
		using type = float;
		using half = Packet4f;
		constexpr static int Size = 8;
		constexpr static int Alignment = 32;
	};

	template<>
	struct unpacket_traits<Packet4f>
	{
		using type = float;
		using half = Packet4f;
		constexpr static int Size = 4;
		constexpr static int Alignment = 16;
	};

	template<>
	struct unpacket_traits<Packet4d>
	{
		using type = double;
		using half = Packet2d;
		constexpr static int Size = 4;
		constexpr static int Alignment = 32;
	};

	template<>
	struct unpacket_traits<Packet2d>
	{
		using type = double;
		using half = Packet2d;
		constexpr static int Size = 2;
		constexpr static int Alignment = 16;
	};

	template<>
	struct unpacket_traits<Packet8i>
	{
		using type = int;
		using half = Packet4i;
		constexpr static int Size = 8;
		constexpr static int Alignment = 32;
	};

	template<>
	struct unpacket_traits<Packet4i>
	{
		using type = int;
		using half = Packet4i;
		constexpr static int Size = 4;
		constexpr static int Alignment = 16;
	};

	template<typename Packet>
	inline Packet pload(const typename unpacket_traits<Packet>::type* from) { return *from; }
	template<typename Packet>
	inline Packet ploadu(const typename unpacket_traits<Packet>::type* from) { return *from; }

	template<> inline Packet4f pload(const float* from) { return _mm_load_ps(from); }
	template<> inline Packet2d pload(const double* from) { return _mm_load_pd(from); }
	template<> inline Packet4i pload(const int* from) { return _mm_load_si128(reinterpret_cast<const __m128i*>(from)); }
	template<> inline Packet4f ploadu(const float* from) { return _mm_loadu_ps(from); }
	template<> inline Packet2d ploadu(const double* from) { return _mm_loadu_pd(from); }
	template<> inline Packet4i ploadu(const int* from) { return _mm_loadu_si128(reinterpret_cast<const __m128i*>(from)); }

	template<typename Scalar, typename Packet>
	inline void pstore(Scalar* to, const Packet& from) { (*to) = from; }
	template<typename Scalar, typename Packet>
	inline void pstoreu(Scalar* to, const Packet& from) { (*to) = from; }

	template<> inline void pstore(float* to, const Packet4f& from) { _mm_store_ps(to, from); }
	template<> inline void pstore(double* to, const Packet2d& from) { _mm_store_pd(to, from); }

	template<> inline void pstoreu(float* to, const Packet4f& from) { _mm_storeu_ps(to, from); }
	template<> inline void pstoreu(double* to, const Packet2d& from) { _mm_storeu_pd(to, from); }

	template<typename Packet, bool IsAligned>
	inline Packet ploadt(const typename unpacket_traits<Packet>::type* from)
	{
		if constexpr(IsAligned)
			return pload<Packet>(from);
		else
			return ploadu<Packet>(from);
	}

	template<typename Packet, bool IsAligned>
	inline void pstoret(typename unpacket_traits<Packet>::type* to, const Packet& from)
	{
		if constexpr (IsAligned)
			pstore(to, from);
		else
			pstoreu(to, from);
	}

	template<typename Packet> inline Packet padd(const Packet& a, const Packet& b) { return a + b; }
	template<> inline bool padd(const bool& a, const bool& b) { return a || b; }
	template<> inline Packet4f padd(const Packet4f& a, const Packet4f& b) { return _mm_add_ps(a, b); }
	template<> inline Packet2d padd(const Packet2d& a, const Packet2d& b) { return _mm_add_pd(a, b); }
	template<> inline Packet4i padd(const Packet4i& a, const Packet4i& b) { return _mm_add_epi32(a, b); }

	template<typename Packet> inline Packet psub(const Packet& a, const Packet& b) { return a - b; }
	template<> inline Packet4f psub(const Packet4f& a, const Packet4f& b) { return _mm_sub_ps(a, b); }
	template<> inline Packet2d psub(const Packet2d& a, const Packet2d& b) { return _mm_sub_pd(a, b); }
	template<> inline Packet4i psub(const Packet4i& a, const Packet4i& b) { return _mm_sub_epi32(a, b); }

	template<typename Packet> inline Packet pmul(const Packet& a, const Packet& b) { return a * b; }
	template<> inline bool pmul(const bool& a, const bool& b) { return a && b; }
	template<> inline Packet4f pmul(const Packet4f& a, const Packet4f& b) { return _mm_mul_ps(a, b); }
	template<> inline Packet2d pmul(const Packet2d& a, const Packet2d& b) { return _mm_mul_pd(a, b); }
	template<> inline Packet4i pmul(const Packet4i& a, const Packet4i& b) { return _mm_mul_epi32(a, b); }
}
#else
namespace TG::Math
{
	inline constexpr int MIN_ALIGN_BYTES = 16;
	inline constexpr int MAX_ALIGN_BYTES = 32;
	inline constexpr int DEFAULT_ALIGN_BYTES = MAX_ALIGN_BYTES;
	inline constexpr bool Support_SIMD = true;
}
#endif