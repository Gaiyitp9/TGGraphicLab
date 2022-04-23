/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "UnitTest.h"
#include <iostream>
#include "Utility.h"
#include "TimeSystem.h"
#include "Math/MathUtil.h"
#include "Math/Vector.hpp"
#include "Input/InputEvent.h"

namespace LCH
{
	void UnitTest::FormatTest()
	{
		InputEvent ie{ KeyCode::BackSlash, InputEvent::Type::Press };
		std::wcout << std::format(L"{:e}", ie) << std::endl;
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
		LCH::TimeSystem timer;
		timer.Tick();
		int i = 1000000;
		while (i-- > 0);
		timer.Tick();

		std::wcout << timer.DeltaTime() << std::endl;
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
		Math::Vector<double, 2> v;
	}
}