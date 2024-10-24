/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Renderer.h"
#include "spdlog/spdlog.h"
#include <fstream>
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl3.h"

namespace TG
{
	Renderer::Renderer()
	{
		CreateEGLDisplay();
		ChooseEGLConfig();
		CreateEGLSurface();
		SetupEGLContext();
		// 查看opengl es版本
        auto glVersion = reinterpret_cast<char const*>(glGetString(GL_VERSION));
		spdlog::info(glVersion);
		// 初始化三角形数据
		InitialTriangle();
		// 开启垂直同步
		eglSwapInterval(m_eglDisplay, 1);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		ImGui::StyleColorsDark();

		ImGui_ImplWin32_Init(m_platformModule.GetWindow().GetWindowHandle());
		ImGui_ImplOpenGL3_Init();

		// 输入模块监听输入事件
		m_platformModule.AddEventListener(m_inputModule.GetMouseEventHandler());
		m_platformModule.AddEventListener(m_inputModule.GetKeyboardEventHandler());
	}

	Renderer::~Renderer()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteProgram(m_shaderProgram);
		eglMakeCurrent(m_eglDisplay, EGL_NO_DISPLAY, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglTerminate(m_eglDisplay);
	}

	int Renderer::Run()
	{
		bool showDemoWindow = true;
		bool showAnotherWindow = false;
		float clearColor[4]{ 0.2f, 0.3f, 0.3f, 1.0f };
		ImGuiIO& io = ImGui::GetIO();

		while (true)
		{
			if (m_platformModule.ShouldExit())
				return m_platformModule.ExitCode();

			m_inputModule.Update();
			m_platformModule.Update();

            if (m_inputModule.GetKeyUp(Input::KeyCode::Space))
                spdlog::info("space up");

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			if (showDemoWindow)
				ImGui::ShowDemoWindow(&showDemoWindow);

			{
				static float f = 0.0f;
				static int counter = 0;

				ImGui::Begin("Hello World!");

				ImGui::Text("This is some useful text.");
				ImGui::Checkbox("Demo Window", &showDemoWindow);
				ImGui::Checkbox("Another Window", &showAnotherWindow);

				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
				ImGui::ColorEdit3("Color", clearColor);

				if (ImGui::Button("Button"))
					++counter;
				ImGui::SameLine();
				ImGui::Text("Counter = %d", counter);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
				ImGui::End();
			}

			if (showAnotherWindow)
			{
				ImGui::Begin("Another Window", &showAnotherWindow);
				ImGui::Text("Hello from another window!");
				if (ImGui::Button("Close Me"))
					showAnotherWindow = false;
				ImGui::End();
			}

			ImGui::Render();

			glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(m_shaderProgram);
			glBindVertexArray(m_VAO);
			glDrawArrays(GL_TRIANGLES, 0, 3);

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
			eglSwapBuffers(m_eglDisplay, m_eglSurface);
		}

		return 0;
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

		std::ifstream vertexFile("../../Shaders/GLSL/simple.vert");
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

		std::ifstream fragmentFile("../../Shaders/GLSL/simple.frag");
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

	bool Renderer::CreateEGLDisplay()
	{
		m_eglDisplay = eglGetDisplay(m_platformModule.GetWindow().GetDisplay());
		if (m_eglDisplay == EGL_NO_DISPLAY)
			m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		if (m_eglDisplay == EGL_NO_DISPLAY)
		{
			spdlog::error("Failed to get an EGLDisplay");
			return false;
		}

		EGLint eglMajorVersion = 0;
		EGLint eglMinorVersion = 0;
		if (eglInitialize(m_eglDisplay, &eglMajorVersion, &eglMinorVersion) != EGL_TRUE)
		{
			spdlog::error("Failed to initialize EGLDisplay: {:#x}", eglGetError());
			return false;
		}
		spdlog::info("EGL Version: {}.{}", eglMajorVersion, eglMinorVersion);

		if (eglBindAPI(EGL_OPENGL_ES_API) != EGL_TRUE)
		{
			spdlog::error("Failed to bind EGL OpenGL ES: {:#x}", eglGetError());
			return false;
		}

		return true;
	}

	bool Renderer::ChooseEGLConfig()
	{
		const EGLint configurationAttributes[] = {
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
			EGL_NONE
		};

		EGLint numConfigs;
		if (eglChooseConfig(m_eglDisplay, configurationAttributes, &m_eglConfig, 1, &numConfigs) != GL_TRUE)
		{
			spdlog::error("eglChooseConfig failed: {:#x}", eglGetError());
			return false;
		}
		if (numConfigs != 1)
		{
			spdlog::error("eglChooseConfig return no config");
			return false;
		}

		return true;
	}

	bool Renderer::CreateEGLSurface()
	{
		m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig, m_platformModule.GetWindow().GetWindowHandle(), nullptr);
		if (m_eglSurface == EGL_NO_SURFACE)
		{
			spdlog::error("Failed to create EGL surface: {:#x}", eglGetError());
			return false;
		}
		return true;
	}

	bool Renderer::SetupEGLContext()
	{
		EGLint contextAttributes[] = {
			EGL_CONTEXT_MAJOR_VERSION, 3,
			EGL_CONTEXT_MINOR_VERSION, 2,
			EGL_NONE
		};
		m_eglContext = eglCreateContext(m_eglDisplay, m_eglConfig, nullptr, contextAttributes);
		if (m_eglContext == EGL_NO_CONTEXT)
		{
			spdlog::error("Failed to create EGL context: {:#x}", eglGetError());
			return false;
		}

		if (eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext) != EGL_TRUE)
		{
			spdlog::error("Failed to make EGL current: {:#x}", eglGetError());
			return false;
		}
		return true;
	}
}
