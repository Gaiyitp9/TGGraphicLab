/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Renderer.hpp"
#include "PAL/Windows/Win32Exception.h"
#include "Exception/BaseException.h"
#include "spdlog/spdlog.h"
#include "glad/gles2.h"

namespace TG
{
	Renderer::Renderer() : m_mainWindow(200, 100, m_windowWidth, m_windowHeight, "天工渲染器")
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
		InitialOpenGLES();
		// 绘制三角形
		InitTriangle();
	}

	Renderer::~Renderer()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteProgram(m_shaderProgram);
	}

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

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(m_shaderProgram);
			glBindVertexArray(m_VAO);
			glDrawArrays(GL_TRIANGLES, 0, 3);
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
			// eglSwapBuffers(m_eglDisplay, m_eglSurface);
			SwapBuffers(m_mainWindow.GetDisplay());
		}

		return 0;
	}

	void Renderer::InitialOpenGLES()
	{
		// int eglVersion = gladLoaderLoadEGL(nullptr);
		// if (eglVersion == 0)
		// 	spdlog::error("Unable to load EGL.");
		// spdlog::info("Loaded EGL {}.{} on first load.",
		//    GLAD_VERSION_MAJOR(eglVersion), GLAD_VERSION_MINOR(eglVersion));
		//
		// PAL::NativeDisplay nativeDisplay = m_mainWindow.GetDisplay();
		// EGLint dispattrs[] =
		// {
		// 	EGL_PLATFORM_ANGLE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_TYPE_VULKAN_ANGLE,
		// 	EGL_NONE
		// };
		// m_eglDisplay = eglGetPlatformDisplayEXT(EGL_PLATFORM_ANGLE_ANGLE,
		// 											  reinterpret_cast<void *>(nativeDisplay), dispattrs);
		// m_eglDisplay = eglGetDisplay(nativeDisplay);
		// if (m_eglDisplay == EGL_NO_DISPLAY)
		// 	spdlog::error("Got no EGL display.");
		//
		// if (!eglInitialize(m_eglDisplay, nullptr, nullptr))
		// 	spdlog::error("Unable to initialize EGL.");
		//
		// eglVersion = gladLoaderLoadEGL(m_eglDisplay);
		// if (eglVersion == 0)
		// 	spdlog::error("Unable to reload EGL.");
		// spdlog::info("Loaded EGL {}.{} after load.",
		//    GLAD_VERSION_MAJOR(eglVersion), GLAD_VERSION_MINOR(eglVersion));
		//
		// if (!eglBindAPI(EGL_OPENGL_ES_API))
		// 	spdlog::error("Unable to bind API.");
		//
		// EGLint configAttributes[] =
		// {
		// 	EGL_BUFFER_SIZE, 0,
		// 	EGL_RED_SIZE, 5,
		// 	EGL_GREEN_SIZE, 6,
		// 	EGL_BLUE_SIZE, 5,
		// 	EGL_ALPHA_SIZE, 0,
		// 	EGL_COLOR_BUFFER_TYPE, EGL_RGB_BUFFER,
		// 	EGL_DEPTH_SIZE, 24,
		// 	EGL_LEVEL, 0,
		// 	EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		// 	EGL_SAMPLE_BUFFERS, 0,
		// 	EGL_SAMPLES, 0,
		// 	EGL_STENCIL_SIZE, 0,
		// 	EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		// 	EGL_TRANSPARENT_TYPE, EGL_NONE,
		// 	EGL_TRANSPARENT_RED_VALUE, EGL_DONT_CARE,
		// 	EGL_TRANSPARENT_GREEN_VALUE, EGL_DONT_CARE,
		// 	EGL_TRANSPARENT_BLUE_VALUE, EGL_DONT_CARE,
		// 	EGL_CONFIG_CAVEAT, EGL_DONT_CARE,
		// 	EGL_CONFIG_ID, EGL_DONT_CARE,
		// 	EGL_MAX_SWAP_INTERVAL, EGL_DONT_CARE,
		// 	EGL_MIN_SWAP_INTERVAL, EGL_DONT_CARE,
		// 	EGL_NATIVE_RENDERABLE, EGL_DONT_CARE,
		// 	EGL_NATIVE_VISUAL_TYPE, EGL_DONT_CARE,
		// 	EGL_NONE
		// };
		// EGLint numConfigs;
		// EGLConfig windowConfig;
		// if (!eglChooseConfig(m_eglDisplay, configAttributes, &windowConfig, 1, &numConfigs))
		// 	spdlog::error("Unable to config");
		//
		// EGLint surfaceAttributes[] = { EGL_NONE };
		// m_eglSurface = eglCreateWindowSurface(m_eglDisplay, windowConfig,
		// 	m_mainWindow.GetWindowHandle(), surfaceAttributes);
		//
		// EGLint contextAttributes[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
		// m_eglContext = eglCreateContext(m_eglDisplay, windowConfig, nullptr, contextAttributes);
		//
		// eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);

		int pixelFormat = ChoosePixelFormat(m_mainWindow.GetDisplay(), &m_pfd);
		SetPixelFormat(m_mainWindow.GetDisplay(), pixelFormat, &m_pfd);
		m_hglrc = wglCreateContext(m_mainWindow.GetDisplay());
		wglMakeCurrent(m_mainWindow.GetDisplay(), m_hglrc);

		int glesVersion = gladLoadGLES2((GLADloadfunc)wglGetProcAddress);
		if (glesVersion == 0)
			spdlog::error("Unable to load GLES.");
		spdlog::info("Loaded GLES {}.{}", GLAD_VERSION_MAJOR(glesVersion), GLAD_VERSION_MINOR(glesVersion));
		glViewport(0, 0, m_windowWidth, m_windowHeight);

	}

	void Renderer::InitTriangle()
	{
		float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		glGenBuffers(1, &m_VBO);
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
		// 把顶点数据传入显存
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		char const* vertexShaderSource = "#version 300 es\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}\0";
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
		glCompileShader(vertexShader);
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
			spdlog::info("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}", infoLog);
		}

		char const* fragmentShaderSource = "#version 300 es\n"
		"precision mediump float;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
			spdlog::info("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n {}", infoLog);
		}

		m_shaderProgram = glCreateProgram();
		glAttachShader(m_shaderProgram, vertexShader);
		glAttachShader(m_shaderProgram, fragmentShader);
		glLinkProgram(m_shaderProgram);
		glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_shaderProgram, 512, nullptr, infoLog);
			spdlog::info("ERROR::SHADER::PROGRAM::LINKING_FAILED\n {}", infoLog);
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
}
