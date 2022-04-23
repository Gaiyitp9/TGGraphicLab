/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "../resource.h"
#include "Application.h"
#include "WindowRegister.h"
#include "Diagnostics/WinAPIException.h"

namespace LCH
{
	Application::Application()
	{
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
		windows[L"天工渲染器"] = std::make_unique<Window>(800, 600, L"天工渲染器");
		windows[L"天工渲染器"]->input.SpyInputEvent();
		windows[L"辅助窗口"] = std::make_unique<Window>(400, 300, L"辅助窗口", windows[L"天工渲染器"]->Hwnd());
		windows[L"辅助窗口"]->SpyMessage();
		windows[L"天工渲染器"]->SetIcon(IDI_ICON1, IDI_ICON1);

		//throw LCH::WinAPIException(E_OUTOFMEMORY);
		unitTest.FormatTest();
		unitTest.TextEncodeTest();
		unitTest.TimeTest();
		unitTest.MathLibTest();
		unitTest.ArrayAlignmentTest();

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
		}
	}
}