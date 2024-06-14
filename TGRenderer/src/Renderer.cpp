/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Renderer.hpp"
#include "PAL/Windows/Win32Exception.h"
#include "spdlog/spdlog.h"
#include "eglext_angle.h"

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

		spdlog::info(Chronometer::Date());

		// 获取当前显示器的宽和高
		m_screenWidth = GetSystemMetrics(SM_CXSCREEN);
        m_screenHeight = GetSystemMetrics(SM_CYSCREEN);

        // 主窗口设置
        m_mainWindow.SetIcon(R"(..\..\TGRenderer\maple-leaf.ico)");
        m_mainWindow.AddInputEventListener(m_input);
        // m_mainWindow.SetStateCallback([&timer=m_timer](){ timer.Start(); }, [&timer=m_timer](){ timer.Pause(); });

		// 初始化OpenGL ES
		int eglVersion = gladLoaderLoadEGL(nullptr);
		if (eglVersion == 0)
			spdlog::error("Unable to load EGL.");
		spdlog::info("Loaded EGL {}.{} on first load.",
		   GLAD_VERSION_MAJOR(eglVersion), GLAD_VERSION_MINOR(eglVersion));

		PAL::NativeDisplay nativeDisplay = m_mainWindow.GetDisplay();
		EGLint dispattrs[] =
		{
			EGL_PLATFORM_ANGLE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_TYPE_VULKAN_ANGLE,
			EGL_NONE
		};
		m_eglDisplay = eglGetPlatformDisplayEXT(EGL_PLATFORM_ANGLE_ANGLE,
													  reinterpret_cast<void *>(nativeDisplay), dispattrs);
		if (m_eglDisplay == EGL_NO_DISPLAY)
			spdlog::error("Got no EGL display.");

		if (!eglInitialize(m_eglDisplay, nullptr, nullptr))
			spdlog::error("Unable to initialize EGL.");

		eglVersion = gladLoaderLoadEGL(m_eglDisplay);
		if (eglVersion == 0)
			spdlog::error("Unable to reload EGL.");
		spdlog::info("Loaded EGL {}.{} after load.",
		   GLAD_VERSION_MAJOR(eglVersion), GLAD_VERSION_MINOR(eglVersion));

		if (!eglBindAPI(EGL_OPENGL_ES_API))
			spdlog::error("Unable to bind API.");

		EGLint configAttributes[] =
		{
			EGL_BUFFER_SIZE, 0,
			EGL_RED_SIZE, 5,
			EGL_GREEN_SIZE, 6,
			EGL_BLUE_SIZE, 5,
			EGL_ALPHA_SIZE, 0,
			EGL_COLOR_BUFFER_TYPE, EGL_RGB_BUFFER,
			EGL_DEPTH_SIZE, 24,
			EGL_LEVEL, 0,
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
			EGL_SAMPLE_BUFFERS, 0,
			EGL_SAMPLES, 0,
			EGL_STENCIL_SIZE, 0,
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_TRANSPARENT_TYPE, EGL_NONE,
			EGL_TRANSPARENT_RED_VALUE, EGL_DONT_CARE,
			EGL_TRANSPARENT_GREEN_VALUE, EGL_DONT_CARE,
			EGL_TRANSPARENT_BLUE_VALUE, EGL_DONT_CARE,
			EGL_CONFIG_CAVEAT, EGL_DONT_CARE,
			EGL_CONFIG_ID, EGL_DONT_CARE,
			EGL_MAX_SWAP_INTERVAL, EGL_DONT_CARE,
			EGL_MIN_SWAP_INTERVAL, EGL_DONT_CARE,
			EGL_NATIVE_RENDERABLE, EGL_DONT_CARE,
			EGL_NATIVE_VISUAL_TYPE, EGL_DONT_CARE,
			EGL_NONE
		};
		EGLint numConfigs;
		EGLConfig windowConfig;
		if (!eglChooseConfig(m_eglDisplay, configAttributes, &windowConfig, 1, &numConfigs))
			spdlog::error("Unable to config");

		EGLint surfaceAttributes[] = { EGL_NONE };
		m_eglSurface = eglCreateWindowSurface(m_eglDisplay, windowConfig,
			m_mainWindow.GetWindowHandle(), surfaceAttributes);

		EGLint contextAttributes[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
		m_eglContext = eglCreateContext(m_eglDisplay, windowConfig, NULL, contextAttributes);

		eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);
	}

	Renderer::~Renderer() = default;

	int Renderer::Run()
	{
		//d3d11Layer = std::make_unique<Graphics::GraphicsLayer>(mainWnd.get());

		while (true)
		{
			if (m_mainWindow.IsDestroyed())
				break;

            m_input.Update();
			if (const std::optional<int> code = PAL::PollEvents())
				return *code;

            if (m_input.GetKeyUp(Input::KeyCode::Space))
                spdlog::info("space up");

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
			eglSwapBuffers(m_eglDisplay, m_eglSurface);
		}

		return 0;
	}
}
