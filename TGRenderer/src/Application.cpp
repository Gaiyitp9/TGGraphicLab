/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "../resource.h"
#include "Application.hpp"
#include "Diagnostics/WinAPIException.hpp"

namespace TG
{
	Application::Application()
	{
		// 开启内存泄漏检测
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		// 设置内存泄漏消息输出到控制台
		_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
		_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);

		// 为了能在控制台查看日志，需要把控制台的代码页(code page)设置为UTF-8
		SetConsoleCP(65001);
		SetConsoleOutputCP(65001);

		// 编码设置为UTF-8
		locale = std::locale(".utf8");
		std::wcout.imbue(locale);
		timer = std::make_shared<Chronometer>();

		// 获取当前显示器的宽和高
		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);
	}

	Application::~Application()
	{

	}

	int Application::Run()
	{
		std::shared_ptr<MainWindow> mainWnd = std::make_shared<MainWindow>(200, 100, 800, 600, L"天工渲染器");
		windows[L"天工渲染器"] = mainWnd;
		mainWnd->SetTimer(timer);
		mainWnd->SpyInputEvent(false);
		mainWnd->SpyMessage(false);
		mainWnd->SetIcon(IDI_ICON1);
		d3d11Layer = std::make_unique<Graphics::GraphicsLayer>(mainWnd.get());

		//windows[L"辅助窗口"] = std::make_shared<PopupWindow>((screenWidth - 400) / 2, (screenHeight - 300) / 2,
		//	400, 300, windows[L"天工渲染器"]);

		//throw TG::WinAPIException(E_OUTOFMEMORY);
		unitTest.FormatTest();
		unitTest.TextEncodeTest();
		unitTest.TimeTest();
		unitTest.MathLibTest();
		unitTest.ArrayAlignmentTest();
		//unitTest.SIMDTest();
		unitTest.OpenGLTest(mainWnd->Hwnd());

		while (true)
		{
			if (const auto code = Window::ProcessMessage())
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

			const float c = sin(timer->TotalTime() * 0.001f) / 2.0f + 0.5f;
			d3d11Layer->ClearBackground(Math::Color::AliceBlue * c);
			d3d11Layer->Update();
#ifdef _DEBUG
			if (mainWnd->Input().GetKeyDown(KeyCode::Space))
				d3d11Layer->dbgInfo->ReportLiveObjects();
#endif
		}
	}
}