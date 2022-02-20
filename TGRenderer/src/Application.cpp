/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Application.h"
#include "Window.h"
#include "WindowRegister.h"
#include "TimeSystem.h"
#include "Utility.h"
#include "Diagnostics/BaseException.h"
#include "../resource.h"
#include <iostream>
#include <format>

namespace LCH
{
	Application::Application()
	{
		WindowRegister::GetInstance()->Initialize({IDI_ICON1, IDI_ICON2});

		// 区域设置为简体中文，编码为UTF-8
		locale = std::locale("zh_CN.utf8");
		std::wcout.imbue(locale);

		// 为了能在控制台查看日志，需要把控制台的代码页(code page)设置为UTF-8
		SetConsoleCP(65001);
		SetConsoleOutputCP(65001);
	}

	Application::~Application()
	{

	}

	int Application::Run()
	{
		LCH::Window wnd1(800, 600, L"天工渲染器");
		LCH::Window wnd2(400, 300, L"辅助窗口", wnd1.GetHwnd());
		LCH::TimeSystem timer;

		const wchar_t* testStr = L"天工渲染器";
		std::wcout << sizeof(wchar_t) << testStr[0] << std::endl;
		const wchar_t* testStr2 = L"TianGong";
		std::wcout << testStr2 << sizeof(testStr2) << std::endl;
		std::wcout << L"你好TG" << std::endl;
		std::string str1 = Utility::WideStringToAnsi(L"你好");
		std::wstring wstr1 = Utility::AnsiToWideString("你好");
		std::cout << str1 << str1.size() << std::endl;
		std::wcout << wstr1 << wstr1.size() << std::endl;

		//throw LCH::BaseException();

		timer.Tick();
		int i = 1000000;
		while (i-- > 0);
		timer.Tick();

		std::wcout << timer.DeltaTime() << std::endl;

		while (true)
		{
			if (const auto code = LCH::Window::ProcessMessage())
			{
				return *code;
			}
		}
	}
}