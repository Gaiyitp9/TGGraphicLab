/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Renderer.hpp"
#include "PAL/Windows/Win32Exception.h"
#include "Log.h"

namespace TG
{
	Renderer::Renderer() : m_mainWindow(200, 100, 800, 600, "天工渲染器")
	{
        // 注：使用CRT library检测内存泄漏时，文件的行分隔符要设置为CRLF(\r\n)，否则_CrtSetDbgFlag函数不起作用
		// 开启内存泄漏检测
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		// 设置内存泄漏消息输出到控制台
		_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
		_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);

		// 为了能在控制台查看日志，需要把控制台的代码页(code page)设置为UTF-8
		SetConsoleCP(65001);
		SetConsoleOutputCP(65001);

		// 编码设置为UTF-8
		std::cout.imbue(m_locale);
		Debug::LogLine(Chronometer::Date());

		// 获取当前显示器的宽和高
		m_screenWidth = GetSystemMetrics(SM_CXSCREEN);
        m_screenHeight = GetSystemMetrics(SM_CYSCREEN);

        // 主窗口设置
        m_mainWindow.SetIcon(R"(..\..\TGRenderer\maple-leaf.ico)");
        // m_mainWindow.SetInputListener([&input = m_input](const Input::Event &evt)
                                      // { input.Broadcast(evt); });
        // m_mainWindow.SetStateCallback([&timer=m_timer](){ timer.Start(); }, [&timer=m_timer](){ timer.Pause(); });
        // m_mainWindow.SpyMessage(false);

        // m_input.SpyEvent<Input::Mouse>(true);
        // m_input.SpyEvent<Input::Keyboard>(true);

        // 初始化opengl
        // Graphics::GLCreateInfo info { 0, m_mainWindow.Hwnd()};
        // Graphics::DeviceContextGL* pContext;
        // Graphics::RenderDeviceGL* pDevice;
        // m_factory.CreateDeviceAndContext(info, &pDevice, &pContext);
        // m_device.reset(pDevice);
        // m_context.reset(pContext);
        // std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
        // glViewport(0, 0, m_mainWindow.Width(), m_mainWindow.Height());
        // wglSwapIntervalEXT(0);      // 关闭垂直同步
	}

	Renderer::~Renderer() = default;

	int Renderer::Run()
	{
		//d3d11Layer = std::make_unique<Graphics::GraphicsLayer>(mainWnd.get());

		//m_windows[L"辅助窗口"] = std::make_shared<PopupWindow>((m_screenWidth - 400) / 2, (m_screenHeight - 300) / 2,
		//	400, 300, m_windows[L"天工渲染器"]);

//		throw TG::Win32Exception(E_OUTOFMEMORY);

		while (true)
		{
			// if (m_mainWindow.Destroy())
				// break;

            // m_input.Update();
			if (const std::optional<int> code = PAL::PollEvents())
				return *code;

            // if (m_input.GetKeyUp(Input::KeyCode::Space))
                // Debug::LogLine(L"space up");

			// const float c = sin(m_timer.TotalTime() * 0.001f) / 2.0f + 0.5f;
			//d3d11Layer->ClearBackground(Math::Color::AliceBlue * c);
			//d3d11Layer->Update();
//#ifdef _DEBUG
//			if (mainWnd->Input().GetKeyDown(KeyCode::Space))
//				d3d11Layer->dbgInfo->ReportLiveObjects();
//#endif
            // Math::Color color = Math::Color::AliceBlue * c;
            // glClearColor(color.r(), color.g(), color.b(), color.a());
            // glClear(GL_COLOR_BUFFER_BIT);
            // SwapBuffers(m_context->m_hdc);
            // std::cout << std::flush;
		}

		return 0;
	}
}
