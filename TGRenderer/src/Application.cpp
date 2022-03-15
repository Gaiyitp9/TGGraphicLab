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
		InputEvent ie{ KeyCode::BackSlash, InputEvent::Type::Press };
		std::wcout << std::format(L"{:e}", ie) << std::endl;

		windows[L"天工渲染器"] = std::make_unique<Window>(800, 600, L"天工渲染器");
		windows[L"天工渲染器"]->input.SpyInputEvent();
		windows[L"辅助窗口"] = std::make_unique<Window>(400, 300, L"辅助窗口", windows[L"天工渲染器"]->Hwnd());
		windows[L"辅助窗口"]->SpyMessage();

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

		//throw LCH::WinAPIException(E_OUTOFMEMORY);

		LCH::TimeSystem timer;
		timer.Tick();
		int i = 1000000;
		while (i-- > 0);
		timer.Tick();

		std::wcout << timer.DeltaTime() << std::endl;

		while (true)
		{
			if (const auto code = LCH::Window::ProcessMessage())
				return *code;

			for (auto it = windows.begin(); it != windows.end(); ++it)
			{
				auto& window = it->second;
				// 如果Windows窗口被销毁，则移除对应的窗口
				while (!window->Exist())
					it = windows.erase(it);
				// 更新窗口
				window->Update();
			}

			/*unsigned int i = 0x7fffffff;
			while (i-- > 1)
			{
				count++;
			}*/
		}
	}
}