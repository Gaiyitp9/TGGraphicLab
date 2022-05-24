/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "SlimWindows.h"
#include "UnitTest.hpp"
#include <iostream>
#include "Utility.hpp"
#include "Math/MathUtil.hpp"
#include "Math/Vector.hpp"
#include "Input/InputEvent.hpp"
#include "Diagnostics/Debug.hpp"

namespace LCH
{
	void UnitTest::FormatTest()
	{
		InputEvent ie{ KeyCode::BackSlash, InputEvent::Type::Press };
		std::wcout << std::format(L"{:k}", ie) << std::endl;
	}

	void UnitTest::TextEncodeTest()
	{
		const wchar_t* unicodeStr = L"\u303E";
		std::wcout << unicodeStr << std::endl;
		const wchar_t* testStr = L"天工渲染器";
		std::wcout << sizeof(wchar_t) << testStr[0] << std::endl;
		const wchar_t* testStr2 = L"TianGong";
		std::wcout << testStr2 << sizeof(testStr2) << std::endl;
		std::wcout << L"你好TG" << std::endl;
		std::string str1 = Utility::WideStringToAnsi(L"你好");
		std::wstring wstr1 = Utility::AnsiToWideString("你好");
		std::cout << str1 << str1.size() << std::endl;
		std::wcout << wstr1 << wstr1.size() << std::endl;
	}

	void UnitTest::TimeTest()
	{
		timer.Tick();
		int i = 1000000;
		while (i-- > 0);
		timer.Tick();

		std::wcout << timer.DeltaTime() << " ms" << std::endl;
	}

	void UnitTest::ArrayAlignmentTest()
	{
		std::array<bool, 10> a;
		std::array<char, 10> b;
		std::array<int, 10> c;
		std::array<long long, 10> d;
		std::array<float, 10> e;
		std::array<double, 10> f;

		Math::aligned_array<bool, 10, 8> a1;
		Math::aligned_array<char, 10> b1;
		Math::aligned_array<int, 10> c1;
		Math::aligned_array<long long, 10> d1;
		Math::aligned_array<float, 10> e1;
		Math::aligned_array<double, 10> f1;

		std::cout << "array<bool,10>.data()       = " << a.data() << std::boolalpha << Math::MathUtil::IsAligned(a.data(), 16) << std::endl;
		std::cout << "array<char,10>.data()       = " << (void*)b.data() << Math::MathUtil::IsAligned(b.data(), 16) << std::endl;
		std::cout << "array<int,10>.data()        = " << c.data() << Math::MathUtil::IsAligned(c.data(), 16) << std::endl;
		std::cout << "array<long long, 10>.data() = " << d.data() << Math::MathUtil::IsAligned(d.data(), 16) << std::endl;
		std::cout << "array<float, 10>.data()     = " << e.data() << Math::MathUtil::IsAligned(e.data(), 16) << std::endl;
		std::cout << "array<double, 10>.data()    = " << f.data() << Math::MathUtil::IsAligned(f.data(), 16) << std::endl;

		std::cout << "aligned_array<bool,10>.data()       = " << a1.data() << std::boolalpha << Math::MathUtil::IsAligned(a1.data(), 8) << std::endl;
		std::cout << "aligned_array<char,10>.data()       = " << (void*)b1.data() << Math::MathUtil::IsAligned(b1.data(), 16) << std::endl;
		std::cout << "aligned_array<int,10>.data()        = " << c1.data() << Math::MathUtil::IsAligned(c1.data(), 16) << std::endl;
		std::cout << "aligned_array<long long, 10>.data() = " << d1.data() << Math::MathUtil::IsAligned(d1.data(), 16) << std::endl;
		std::cout << "aligned_array<float, 10>.data()     = " << e1.data() << Math::MathUtil::IsAligned(e1.data(), 16) << std::endl;
		std::cout << "aligned_array<double, 10>.data()    = " << f1.data() << Math::MathUtil::IsAligned(f1.data(), 16) << std::endl;
	}

	void UnitTest::MathLibTest()
	{
		Math::Vector4f v;
		std::cout << "Vector<float, 4> value address = " << &v << std::boolalpha << Math::MathUtil::IsAligned(&v, 16) << std::endl;
		std::cout << v.x() << " " << v.y() << " " << v.z() << " " << v.w() << std::endl;
		v.x() = 2.3f;
		v.y() = 1.2f;
		v.z() = 3.6f;
		v.w() = 1.0f;
		std::cout << v.x() << " " << v.y() << " " << v.z() << " " << v.w() << std::endl;
		std::cout << "v magnitude: " << v.magnitude() << std::endl;
		Math::Vector4f vn = v.normalized();
		std::cout << "v normalized: " << vn.x() << " " << vn.y() << " " << vn.z() << " " << vn.w() << std::endl;

		Math::Vector4f v1;
		std::cout << v1[0] << " " << v1[1] << " " << v1[2] << " " << v1[3] << std::endl;
		v1[0] = 2.2f;
		v1[1] = 5.3f;
		v1[2] = 8.2f;
		v1[3] = 1.2f;
		std::cout << v1[0] << " " << v1[1] << " " << v1[2] << " " << v1[3] << std::endl;
		std::cout << "v1 magnitude: " << v1.magnitude() << std::endl;
		std::cout << "v dot v1: " << v.Dot(v1) << std::endl;
		Math::Vector4f v1d = 2 * v1;
		std::cout << v1d[0] << " " << v1d[1] << " " << v1d[2] << " " << v1d[3] << std::endl;

		Math::Vector<float, 15> v2;
		Math::Vector<float, 15> v3;
		for (size_t i = 0; i < 8; ++i)
		{
			v2[i] = 1;
			v3[i] = 2;
		}
		for (size_t i = 8; i < 15; ++i)
		{
			v2[i] = 1.2f;
			v3[i] = 2.3f;
		}
		Math::Vector<float, 15> v4 = v2 + v3;
		for (size_t i = 0; i < 15; ++i)
		{
			std::cout << v4[i] << " ";
		}
		std::cout << std::endl;
	}

	void UnitTest::SIMDTest()
	{
		size_t n = 10000000;
		int* nums = static_cast<int*>(_aligned_malloc(n * sizeof(int), 32));
		float* numsf = static_cast<float*>(_aligned_malloc(n * sizeof(float), 32));
		std::cout << "SIMDTest nums = " << nums << " Is 32-byte Aligned? " << Math::MathUtil::IsAligned(nums, 32) << std::endl;

		for (size_t i = 0; i < n; i++)
		{
			nums[i] = 3;
			numsf[i] = 3;
		}

		NormalAdd(nums, n);
		SSEAdd(nums, n);
		AVX2Add(nums, n);
		NormalAddf(numsf, n);
		SSEAddf(numsf, n);
		AVX2Addf(numsf, n);

		_aligned_free(nums);
		_aligned_free(numsf);
	}

	void UnitTest::NormalAdd(int* nums, size_t n)
	{
		timer.Tick();
		int sum = 0;
		for (size_t i = 0; i < n; i++)
		{
			sum += nums[i];
		}
		timer.Tick();
		std::cout << "NormalAdd: " << timer.DeltaTime() << " ms, result = " << sum << std::endl;
	}

	void UnitTest::NormalAddf(float* nums, size_t n)
	{
		timer.Tick();
		float sum = 0;
		for (size_t i = 0; i < n; i++)
		{
			sum += nums[i];
		}
		timer.Tick();
		std::cout << "NormalAddf: " << timer.DeltaTime() << " ms, result = " << sum << std::endl;
	}

	void UnitTest::SSEAdd(int* nums, size_t n)
	{
		timer.Tick();

		__m128i simd_sum = _mm_setzero_si128();
		__m128i simd_load;
		int normal_sum = 0;

		size_t loop = n / 4;
		size_t reserve = n % 4;

		__m128i* p = (__m128i*)nums;
		for (size_t i = 0; i < loop; i++)
		{
			simd_load = _mm_load_si128((__m128i const*)p);
			simd_sum = _mm_add_epi32(simd_sum, simd_load);
			p++;
		}

		int* q = (int*)p;
		for (size_t i = 0; i < reserve; i++)
		{
			normal_sum += q[i];
		}
		normal_sum += (((int*)&simd_sum)[0] + ((int*)&simd_sum)[1] + ((int*)&simd_sum)[2] + ((int*)&simd_sum)[3]);

		timer.Tick();
		std::cout << "SSEAdd: " << timer.DeltaTime() << " ms, result = " << normal_sum << std::endl;
	}

	void UnitTest::SSEAddf(float* nums, size_t n)
	{
		timer.Tick();

		__m128 simd_sum = _mm_setzero_ps();
		__m128 simd_load;
		float normal_sum = 0;

		size_t loop = n / 4;
		size_t reserve = n % 4;

		__m128* p = (__m128*)nums;
		for (size_t i = 0; i < loop; i++)
		{
			simd_load = _mm_load_ps((float const*)p);
			simd_sum = _mm_add_ps(simd_sum, simd_load);
			p++;
		}

		float* q = (float*)p;
		for (size_t i = 0; i < reserve; i++)
		{
			normal_sum += q[i];
		}
		normal_sum += (((float*)&simd_sum)[0] + ((float*)&simd_sum)[1] + ((float*)&simd_sum)[2] + ((float*)&simd_sum)[3]);

		timer.Tick();
		std::cout << "SSEAddf: " << timer.DeltaTime() << " ms, result = " << normal_sum << std::endl;
	}

	void UnitTest::AVX2Add(int* nums, size_t n)
	{
		timer.Tick();

		__m256i simd_sum = _mm256_setzero_si256();
		__m256i simd_load;
		int normal_sum = 0;

		size_t loop = n / 8;
		size_t reserve = n % 8;

		__m256i* p = (__m256i*)nums;
		for (size_t i = 0; i < loop; i++)
		{
			simd_load = _mm256_load_si256((const __m256i*)p);
			simd_sum = _mm256_add_epi32(simd_sum, simd_load);
			p++;
		}

		int* q = (int*)p;
		for (size_t i = 0; i < reserve; i++)
		{
			normal_sum += q[i];
		}
		normal_sum += (((int*)&simd_sum)[0] + ((int*)&simd_sum)[1] +
			((int*)&simd_sum)[2] + ((int*)&simd_sum)[3] +
			((int*)&simd_sum)[4] + ((int*)&simd_sum)[5] +
			((int*)&simd_sum)[6] + ((int*)&simd_sum)[7]);

		timer.Tick();
		std::cout << "AVX2Add: " << timer.DeltaTime() << " ms, result = " << normal_sum << std::endl;
	}

	void UnitTest::AVX2Addf(float* nums, size_t n)
	{
		timer.Tick();

		__m256 simd_sum = _mm256_setzero_ps();
		__m256 simd_load;
		float normal_sum = 0;

		size_t loop = n / 8;
		size_t reserve = n % 8;

		__m256* p = (__m256*)nums;
		for (size_t i = 0; i < loop; i++)
		{
			simd_load = _mm256_load_ps((float const*)p);
			simd_sum = _mm256_add_ps(simd_sum, simd_load);
			p++;
		}

		int* q = (int*)p;
		for (size_t i = 0; i < reserve; i++)
		{
			normal_sum += q[i];
		}
		normal_sum += (((float*)&simd_sum)[0] + ((float*)&simd_sum)[1] +
			((float*)&simd_sum)[2] + ((float*)&simd_sum)[3] +
			((float*)&simd_sum)[4] + ((float*)&simd_sum)[5] +
			((float*)&simd_sum)[6] + ((float*)&simd_sum)[7]);

		timer.Tick();
		std::cout << "AVX2Addf: " << timer.DeltaTime() << " ms, result = " << normal_sum << std::endl;
	}
}