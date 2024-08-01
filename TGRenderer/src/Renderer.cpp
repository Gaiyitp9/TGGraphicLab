/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Renderer.hpp"
#include "PAL/Windows/Win32Exception.h"
#include "Exception/BaseException.h"
#include "spdlog/spdlog.h"
#include "glad/wgl.h"
#include "glad/gles2.h"
#include <fstream>

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

		InitialWgl();
		// 初始化OpenGL ES
		InitialOpenGLES();
		// 绘制三角形
		InitialTriangle();
	}

	Renderer::~Renderer()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteProgram(m_shaderProgram);
		wglDeleteContext(m_hglrc);
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

	static GLADapiproc GLESGetAddress(const char* name)
	{
		static HMODULE openglModule = LoadLibraryA("opengl32.dll");
		auto result = reinterpret_cast<GLADapiproc>(GetProcAddress(openglModule, name));
		if (result == nullptr)
			result = reinterpret_cast<GLADapiproc>(wglGetProcAddress(name));
		return result;
	}

	static LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch(msg)
		{
			case WM_CREATE:
			{
				PIXELFORMATDESCRIPTOR pfd =
				{
					sizeof(PIXELFORMATDESCRIPTOR),
					1,
					PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
					PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
					32,                   // Colordepth of the framebuffer.
					0, 0, 0, 0, 0, 0,
					0,
					0,
					0,
					0, 0, 0, 0,
					24,                   // Number of bits for the depthbuffer
					8,                    // Number of bits for the stencilbuffer
					0,                    // Number of Aux buffers in the framebuffer.
					PFD_MAIN_PLANE,
					0,
					0, 0, 0
				};

				HDC hdc = GetDC(hwnd);

				int pixelFormat = ChoosePixelFormat(hdc, &pfd);
				SetPixelFormat(hdc, pixelFormat, &pfd);

				HGLRC hglrc = wglCreateContext(hdc);
				wglMakeCurrent(hdc, hglrc);

				gladLoaderLoadWGL(hdc);

				wglDeleteContext(hglrc);
				PostQuitMessage(0);
				return 0;
			}

			default:
				return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}

	void Renderer::InitialWgl()
	{
		MSG msg{};
		WNDCLASSEXW wc{};
		wc.cbSize = sizeof(WNDCLASSEXW);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = WndProc;
		wc.lpszClassName = L"dummy";
		if (RegisterClassExW(&wc) == 0)
			PAL::CheckLastError();

		HWND hwnd = CreateWindowExW(0, wc.lpszClassName, nullptr, 0, 0, 0, 0, 0, nullptr, nullptr, nullptr, nullptr);
		if (hwnd == nullptr)
			PAL::CheckLastError();

		while(GetMessageW(&msg, nullptr, 0, 0))
			DispatchMessage(&msg);

		UnregisterClassW(wc.lpszClassName, nullptr);

		int pixelFormat;
		UINT numFormats;
		int formatAttribList[] =
		{
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB, 32,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			0,
		};
		wglChoosePixelFormatARB(m_mainWindow.GetDisplay(), formatAttribList, nullptr, 1, &pixelFormat, &numFormats);
		SetPixelFormat(m_mainWindow.GetDisplay(), pixelFormat, &m_pfd);

		int contextAttribList[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 2,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_ES2_PROFILE_BIT_EXT,
			0,
		};
		m_hglrc = wglCreateContextAttribsARB(m_mainWindow.GetDisplay(), nullptr, contextAttribList);
		wglMakeCurrent(m_mainWindow.GetDisplay(), m_hglrc);
	}

	void Renderer::InitialOpenGLES()
	{
		int glesVersion = gladLoadGLES2(GLESGetAddress);
		if (glesVersion == 0)
			spdlog::error("Unable to load GLES.");
		spdlog::info("Loaded GLES {}.{}", GLAD_VERSION_MAJOR(glesVersion), GLAD_VERSION_MINOR(glesVersion));
		const unsigned char* version = glGetString(GL_VERSION);
		glViewport(0, 0, m_windowWidth, m_windowHeight);
	}

	void Renderer::InitialTriangle()
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

		std::ifstream vertexFile("../../Source/Shaders/GLSL/simple.vert");
		if (!vertexFile)
		{
			spdlog::error("Load simple vertex file failed!");
			return;
		}
		std::stringstream vertexBuffer;
		vertexBuffer << vertexFile.rdbuf();
		std::string vertexStr = vertexBuffer.str();
		char const* vertexShaderSource = vertexStr.c_str();
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
		glCompileShader(vertexShader);
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
			spdlog::error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}", infoLog);
		}

		std::ifstream fragmentFile("../../Source/Shaders/GLSL/simple.frag");
		if (!fragmentFile)
		{
			spdlog::error("Load simple fragment file failed!");
			return;
		}
		std::stringstream fragBuffer;
		fragBuffer << fragmentFile.rdbuf();
		std::string fragStr = fragBuffer.str();
		char const* fragmentShaderSource = fragStr.c_str();
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
			spdlog::error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n {}", infoLog);
		}

		m_shaderProgram = glCreateProgram();
		glAttachShader(m_shaderProgram, vertexShader);
		glAttachShader(m_shaderProgram, fragmentShader);
		glLinkProgram(m_shaderProgram);
		glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_shaderProgram, 512, nullptr, infoLog);
			spdlog::error("ERROR::SHADER::PROGRAM::LINKING_FAILED\n {}", infoLog);
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
}
