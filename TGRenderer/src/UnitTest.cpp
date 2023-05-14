/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "UnitTest.hpp"

namespace TG
{
	void UnitTest::FormatTest()
	{
		Input::Event ie{ Input::DeviceType::Keyboard, Input::KeyCode::BackSlash, Input::EventType::Press , {}};
        Debug::Log(std::format(L"Key: {:<10} Event: {:10} ", Input::EventInfo::keysName[ie.key],
                               Input::EventInfo::eventTypes[ie.type]));
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
		/*std::array<bool, 10> a;
		std::array<char, 10> b;
		std::array<int, 10> c;
		std::array<long long, 10> d;
		std::array<float, 10> e;
		std::array<double, 10> f;*/

		/*Math::aligned_array<bool, 10, 8> a1;
		Math::aligned_array<char, 10> b1;
		Math::aligned_array<int, 10> c1;
		Math::aligned_array<long long, 10> d1;
		Math::aligned_array<float, 10> e1;
		Math::aligned_array<double, 10> f1;

		std::cout << "array<bool,10>.data()       = " << a.data() << std::boolalpha << Math::IsAligned(a.data(), 16) << std::endl;
		std::cout << "array<char,10>.data()       = " << (void*)b.data() << Math::IsAligned(b.data(), 16) << std::endl;
		std::cout << "array<int,10>.data()        = " << c.data() << Math::IsAligned(c.data(), 16) << std::endl;
		std::cout << "array<long long, 10>.data() = " << d.data() << Math::IsAligned(d.data(), 16) << std::endl;
		std::cout << "array<float, 10>.data()     = " << e.data() << Math::IsAligned(e.data(), 16) << std::endl;
		std::cout << "array<double, 10>.data()    = " << f.data() << Math::IsAligned(f.data(), 16) << std::endl;

		std::cout << "aligned_array<bool,10>.data()       = " << a1.data() << std::boolalpha << Math::IsAligned(a1.data(), 8) << std::endl;
		std::cout << "aligned_array<char,10>.data()       = " << (void*)b1.data() << Math::IsAligned(b1.data(), 16) << std::endl;
		std::cout << "aligned_array<int,10>.data()        = " << c1.data() << Math::IsAligned(c1.data(), 16) << std::endl;
		std::cout << "aligned_array<long long, 10>.data() = " << d1.data() << Math::IsAligned(d1.data(), 16) << std::endl;
		std::cout << "aligned_array<float, 10>.data()     = " << e1.data() << Math::IsAligned(e1.data(), 16) << std::endl;
		std::cout << "aligned_array<double, 10>.data()    = " << f1.data() << Math::IsAligned(f1.data(), 16) << std::endl;*/
	}

	void UnitTest::MathLibTest()
	{
		Math::Vector4f v1, v2, v3;
		Math::RowVector4f v4;
		v1[0] = 0; v1[1] = 1.5f; v1[2] = 2; v1[3] = 3;
		v2[0] = 0; v2[1] = 2.2f; v2[2] = 2; v2[3] = 3;
		v3[0] = 0; v3[1] = 1.6f; v3[2] = 2; v3[3] = 3;
		v4[0] = 2; v4[1] = 4.8f; v4[2] = 2; v4[3] = 3;
		Math::Vector4f result;
		result = v1 + v2 + v3;
		std::cout << "result: " << result.x() << " " << result.y() << " " 
			<< result.z() << " " << result.w() << std::endl;
		std::cout << "result magnitude: " << result.magnitude() << " sqrMagnitude: " << result.sqrMagnitude() << std::endl;
		Math::Vector4f resultN = result.normalized();
		std::cout << "result normalize: " << resultN.x() << " " << resultN.y() << " "
			<< resultN.z() << " " << resultN.w() << std::endl;
		result.Normalize();
		std::cout << "result: " << result.x() << " " << result.y() << " "
			<< result.z() << " " << result.w() << std::endl;

		float dot = v1.Dot(v2);
		std::cout << "dot: " << dot << std::endl;
		Math::Matrix4f identity = Math::Matrix4f::identity();
		std::cout << "identity: " << std::endl;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
				std::cout << identity(i, j) << " ";
			std::cout << std::endl;
		}

		Math::Matrix<float, 5, 1> v5, v6;
		v5[0] = 0; v5[1] = 1; v5[2] = 2.6f; v5[3] = 3; v5[4] = 5;
		v6[0] = 0; v6[1] = 6; v6[2] = 2.9f; v6[3] = 3; v6[4] = -2;
		Math::Matrix<float, 5, 1> result1;
		result1 = v5 + v6;
		std::cout << "result: " << result1[0] << " " << result1[1] << " "
			<< result1[2] << " " << result1[3] << " " << result1[4] << std::endl;
		dot = v5.Dot(v6);
		std::cout << "dot: " << dot << std::endl;

		Math::Matrix4f vp, view, proj;
		view(0, 0) = 1.0f; view(0, 1) = 0.0f; view(0, 2) = 3.0f; view(0, 3) = 0.0f;
		view(1, 0) = 0.0f; view(1, 1) = 1.0f; view(1, 2) = 0.0f; view(1, 3) = 1.0f;
		view(2, 0) = 0.0f; view(2, 1) = 0.0f; view(2, 2) = 1.0f; view(2, 3) = 0.0f;
		view(3, 0) = 1.0f; view(3, 1) = 0.0f; view(3, 2) = 0.0f; view(3, 3) = 1.0f;

		proj(0, 0) = 1.0f; proj(0, 1) = 0.0f; proj(0, 2) = 0.0f; proj(0, 3) = 0.0f;
		proj(1, 0) = 0.0f; proj(1, 1) = 1.0f; proj(1, 2) = 0.0f; proj(1, 3) = 0.0f;
		proj(2, 0) = 0.0f; proj(2, 1) = 0.0f; proj(2, 2) = 1.0f; proj(2, 3) = 0.0f;
		proj(3, 0) = 1.0f; proj(3, 1) = 0.0f; proj(3, 2) = 0.0f; proj(3, 3) = 1.0f;

		vp = proj * view;
		std::cout << "vp: " << std::endl;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
				std::cout << vp(i, j) << " ";
			std::cout << std::endl;
		}

		Math::Matrix<float, 5, 5> vp1, view1, proj1;
		view1(0, 0) = 1.0f; view1(0, 1) = 0.0f; view1(0, 2) = 3.0f; view1(0, 3) = 0.0f; view1(0, 4) = 0.0f;
		view1(1, 0) = 0.0f; view1(1, 1) = 1.0f; view1(1, 2) = 0.0f; view1(1, 3) = 1.0f; view1(1, 4) = 0.0f;
		view1(2, 0) = 0.0f; view1(2, 1) = 0.0f; view1(2, 2) = 1.0f; view1(2, 3) = 0.0f; view1(2, 4) = 0.0f;
		view1(3, 0) = 1.0f; view1(3, 1) = 0.0f; view1(3, 2) = 0.0f; view1(3, 3) = 1.0f; view1(3, 4) = 0.0f;
		view1(4, 0) = 1.0f; view1(4, 1) = 0.0f; view1(4, 2) = 0.0f; view1(4, 3) = 1.0f; view1(4, 4) = 1.0f;

		proj1(0, 0) = 1.0f; proj1(0, 1) = 0.0f; proj1(0, 2) = 0.0f; proj1(0, 3) = 0.0f; proj1(0, 4) = 0.0f;
		proj1(1, 0) = 0.0f; proj1(1, 1) = 1.0f; proj1(1, 2) = 0.0f; proj1(1, 3) = 0.0f; proj1(1, 4) = 0.0f;
		proj1(2, 0) = 0.0f; proj1(2, 1) = 0.0f; proj1(2, 2) = 1.0f; proj1(2, 3) = 0.0f; proj1(2, 4) = 0.0f;
		proj1(3, 0) = 1.0f; proj1(3, 1) = 0.0f; proj1(3, 2) = 0.0f; proj1(3, 3) = 1.0f; proj1(3, 4) = 0.0f;
		proj1(4, 0) = 1.0f; proj1(4, 1) = 0.0f; proj1(4, 2) = 0.0f; proj1(4, 3) = 1.0f; proj1(4, 4) = 1.0f;
		vp1 = proj1 * view1;
		std::cout << "vp1: " << std::endl;
		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 5; ++j)
				std::cout << vp1(i, j) << " ";
			std::cout << std::endl;
		}
		vp1 = vp1 * 2.0f;
		std::cout << "vp1: " << std::endl;
		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 5; ++j)
				std::cout << vp1(i, j) << " ";
			std::cout << std::endl;
		}
		vp1 = 2.0f * vp1;
		std::cout << "vp1: " << std::endl;
		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 5; ++j)
				std::cout << vp1(i, j) << " ";
			std::cout << std::endl;
		}
		vp1 = vp1 / 2.0f;
		std::cout << "vp1: " << std::endl;
		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 5; ++j)
				std::cout << vp1(i, j) << " ";
			std::cout << std::endl;
		}
	}

	UnitTest::UnitTest()
	{
//		if (glInst == nullptr)
//			CheckLastError();
	}

	UnitTest::~UnitTest()
	{
//		FreeLibrary(glInst);
        gladLoaderUnloadGL();
	}

//	HMODULE UnitTest::glInst = LoadLibraryA("opengl32.dll");
//	UnitTest::PFN_wglGetProcAddress UnitTest::wglGetProcAddress = (PFN_wglGetProcAddress)GetProcAddress(glInst, "wglGetProcAddress");
//	static GLADapiproc GetGLProcAddress(const char* name)
//	{
//		GLADapiproc proc = reinterpret_cast<GLADapiproc>(UnitTest::wglGetProcAddress(name));
//		if (proc)
//			return proc;
//
//		return reinterpret_cast<GLADapiproc>(GetProcAddress(UnitTest::glInst, name));
//	}

	void UnitTest::OpenGLTest(HWND hWnd)
	{
		HDC hdc = GetDC(hWnd);
		HGLRC hglrc;

		PIXELFORMATDESCRIPTOR pfd;
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 24;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;
		int pxfmt = ChoosePixelFormat(hdc, &pfd);
		SetPixelFormat(hdc, pxfmt, &pfd);

		hglrc = wglCreateContext(hdc);
		wglMakeCurrent(hdc, hglrc);

//		gladLoadGL(GetGLProcAddress);
        gladLoaderLoadGL();
		std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	}

	/*void UnitTest::SIMDTest()
	{
		size_t n = 10000000;
		int* nums = static_cast<int*>(_aligned_malloc(n * sizeof(int), 32));
		float* numsf = static_cast<float*>(_aligned_malloc(n * sizeof(float), 32));
		std::cout << "SIMDTest nums = " << nums << " Is 32-byte Aligned? " << Math::IsAligned(nums, 32) << std::endl;

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
		m_timer.Tick();
		int sum = 0;
		for (size_t i = 0; i < n; i++)
		{
			sum += nums[i];
		}
		m_timer.Tick();
		std::cout << "NormalAdd: " << m_timer.DeltaTime() << " ms, result = " << sum << std::endl;
	}

	void UnitTest::NormalAddf(float* nums, size_t n)
	{
		m_timer.Tick();
		float sum = 0;
		for (size_t i = 0; i < n; i++)
		{
			sum += nums[i];
		}
		m_timer.Tick();
		std::cout << "NormalAddf: " << m_timer.DeltaTime() << " ms, result = " << sum << std::endl;
	}

	void UnitTest::SSEAdd(int* nums, size_t n)
	{
		m_timer.Tick();

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

		m_timer.Tick();
		std::cout << "SSEAdd: " << m_timer.DeltaTime() << " ms, result = " << normal_sum << std::endl;
	}

	void UnitTest::SSEAddf(float* nums, size_t n)
	{
		m_timer.Tick();

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

		m_timer.Tick();
		std::cout << "SSEAddf: " << m_timer.DeltaTime() << " ms, result = " << normal_sum << std::endl;
	}

	void UnitTest::AVX2Add(int* nums, size_t n)
	{
		m_timer.Tick();

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

		m_timer.Tick();
		std::cout << "AVX2Add: " << m_timer.DeltaTime() << " ms, result = " << normal_sum << std::endl;
	}

	void UnitTest::AVX2Addf(float* nums, size_t n)
	{
		m_timer.Tick();

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

		m_timer.Tick();
		std::cout << "AVX2Addf: " << m_timer.DeltaTime() << " ms, result = " << normal_sum << std::endl;
	}*/
}