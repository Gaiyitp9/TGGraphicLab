/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "../resource.h"
#include "Application.h"
#include "Diagnostics/WinAPIException.h"

namespace LCH
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

		// 获取当前显示器的宽和高
		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);
	}

	Application::~Application()
	{

	}

	int Application::Run()
	{
		windows[L"天工渲染器"] = std::make_unique<Window>(200, 100, 800, 600, L"天工渲染器");
		windows[L"天工渲染器"]->SpyInputEvent(false);
		windows[L"辅助窗口"] = std::make_unique<Window>((screenWidth - 400) / 2, (screenHeight - 300) / 2, 
			400, 300, L"辅助窗口", windows[L"天工渲染器"].get());
		windows[L"辅助窗口"]->SpyMessage(false);
		// 调用了SetWindowLongPtrW之后需要调用ShowWindow来激活窗口
		SetWindowLongPtrW(windows[L"辅助窗口"]->Hwnd(), GWL_STYLE, WS_POPUP);
		ShowWindow(windows[L"辅助窗口"]->Hwnd(), SW_SHOW);
		windows[L"天工渲染器"]->SetIcon(IDI_ICON1);

		//throw LCH::WinAPIException(E_OUTOFMEMORY);
		unitTest.FormatTest();
		unitTest.TextEncodeTest();
		unitTest.TimeTest();
		unitTest.MathLibTest();
		unitTest.ArrayAlignmentTest();
		unitTest.SIMDTest();

		Graphics::GraphicsLayer<Graphics::LowLevelAPI::DirectX11> d3d11Layer(*windows[L"天工渲染器"]);
		Graphics::GraphicsLayer<Graphics::LowLevelAPI::DirectX11> d3d11Layer2(*windows[L"辅助窗口"]);

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

			const float c = sin(timer.TotalTime() * 0.001f) / 2.0f + 0.5f;
			d3d11Layer.ClearRenderTarget(c, c * 0.5f, 1.0f);
			d3d11Layer.EndFrame();
			d3d11Layer2.ClearRenderTarget(c, c, 1.0f);
			d3d11Layer2.EndFrame();
		}
	}
}