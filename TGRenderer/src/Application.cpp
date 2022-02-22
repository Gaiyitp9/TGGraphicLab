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
#include "Diagnostics/WinAPIException.h"
#include "../resource.h"

namespace LCH
{
	Application::Application()
	{
		WindowRegister::GetInstance()->Initialize({IDI_ICON1, IDI_ICON2});

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
		LCH::Window wnd1(800, 600, L"天工渲染器");
		wnd1.spyMessage = true;
		LCH::Window wnd2(400, 300, L"辅助窗口", wnd1.GetHwnd());
		wnd2.spyMessage = true;
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

		//throw new LCH::WinAPIException(E_OUTOFMEMORY);

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