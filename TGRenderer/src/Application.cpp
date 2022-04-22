/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "../resource.h"
#include "Application.h"
#include "WindowRegister.h"
#include "Utility.h"
#include "Diagnostics/WinAPIException.h"
#include <array>
#include "Math/MathUtil.h"
#include "Math/Vector.hpp"

namespace LCH
{
	Application::Application()
	{
		// 设置应用的icon
		WindowRegister::GetInstance().Initialize({IDI_ICON1, IDI_ICON2});

		// 为了能在控制台查看日志，需要把控制台的代码页(code page)设置为UTF-8
		SetConsoleCP(65001);
		SetConsoleOutputCP(65001);

		// 编码设置为UTF-8
		locale = std::locale(".utf8");
		std::wcout.imbue(locale);
	}

	Application::~Application()
	{

	}

	int Application::Run()
	{
		Math::Vector<double, 2> v;
		InputEvent ie{ KeyCode::BackSlash, InputEvent::Type::Press };
		std::wcout << std::format(L"{:e}", ie) << std::endl;

		windows[L"天工渲染器"] = std::make_unique<Window>(800, 600, L"天工渲染器");
		windows[L"天工渲染器"]->input.SpyInputEvent();
		windows[L"辅助窗口"] = std::make_unique<Window>(400, 300, L"辅助窗口", windows[L"天工渲染器"]->Hwnd());
		windows[L"辅助窗口"]->SpyMessage();

		TextEncodeTest();
		TimeTest();
		ArrayAlignmentTest();
		//throw LCH::WinAPIException(E_OUTOFMEMORY);

		while (true)
		{
			if (const auto code = LCH::Window::ProcessMessage())
				return *code;

			auto it = windows.begin();
			while (it != windows.end())
			{
				// 如果Windows窗口被销毁，则移除对应的窗口
				if (it->second->Destroy()) 
					it = windows.erase(it);
				else
				{
					// 更新窗口
					it->second->Update();
					++it;
				}
			}

			/*unsigned int i = 0x7fffffff;
			while (i-- > 1)
			{
				count++;
			}*/
		}
	}

	void Application::TextEncodeTest()
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

	void Application::TimeTest()
	{
		LCH::TimeSystem timer;
		timer.Tick();
		int i = 1000000;
		while (i-- > 0);
		timer.Tick();

		std::wcout << timer.DeltaTime() << std::endl;
	}

	void Application::ArrayAlignmentTest()
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
}