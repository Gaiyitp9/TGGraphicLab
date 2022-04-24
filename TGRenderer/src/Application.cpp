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

		// 获取当前显示器的宽和高
		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);
	}

	Application::~Application()
	{

	}

	int Application::Run()
	{
		windows[L"天工渲染器"] = std::make_unique<Window>(800, 600, L"天工渲染器");
		windows[L"天工渲染器"]->SpyInputEvent(false);
		windows[L"辅助窗口"] = std::make_unique<Window>(400, 300, L"辅助窗口", windows[L"天工渲染器"].get());
		windows[L"辅助窗口"]->SpyMessage(false);
		SetWindowLongPtrW(windows[L"辅助窗口"]->Hwnd(), GWL_STYLE, WS_POPUP | WS_BORDER);
		SetWindowPos(windows[L"辅助窗口"]->Hwnd(), HWND_TOPMOST, 
			(screenWidth - 400) / 2, (screenHeight - 300) / 2, 400, 300, 0);
		windows[L"天工渲染器"]->SetIcon(IDI_ICON1);

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

			if (windows[L"天工渲染器"]->Input().GetKeyDown(KeyCode::S))
			{
				windows[L"天工渲染器"]->SetIcon(IDI_ICON1);
				Debug::LogLine(L"按下S");
			}
			else if (windows[L"天工渲染器"]->Input().GetKeyDown(KeyCode::D))
			{
				windows[L"天工渲染器"]->SetIcon(IDI_ICON2);
				Debug::LogLine(L"按下D");
			}
			else if (windows[L"天工渲染器"]->Input().GetKey(KeyCode::RightMouseButton))
			{
				Debug::LogLine(L"按住鼠标右键");
			}
		}
	}
}