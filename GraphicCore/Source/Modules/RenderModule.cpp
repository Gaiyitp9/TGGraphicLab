/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include <fstream>
#include <regex>
#include "Modules/RenderModule.h"
#include "spdlog/spdlog.h"
#include "Exception/EGLException.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl3.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace TG
{
    RenderModule::RenderModule() = default;
    RenderModule::~RenderModule()
    {
    	ImGui_ImplOpenGL3_Shutdown();
    	ImGui_ImplWin32_Shutdown();
    	ImGui::DestroyContext();

    	glDeleteVertexArrays(1, &m_VAO);
    	glDeleteBuffers(1, &m_VBO);
    	glDeleteProgram(m_shaderProgram);

    	eglDestroySurface(m_eglDisplay, m_eglSurface);
    	eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    	eglDestroyContext(m_eglDisplay, m_eglContext);
    	eglTerminate(m_eglDisplay);
    }

    void RenderModule::Update()
    {
    	float clearColor[4]{ 0.2f, 0.3f, 0.3f, 1.0f };
    	ImGuiIO& io = ImGui::GetIO();

    	ImGui_ImplOpenGL3_NewFrame();
    	ImGui_ImplWin32_NewFrame();
    	ImGui::NewFrame();

    	if (m_showDemoWindow)
    		ImGui::ShowDemoWindow(&m_showDemoWindow);

	    {
    		static float f = 0.0f;
    		static int counter = 0;

    		ImGui::Begin("Hello World!");

    		ImGui::Text("This is some useful text.");
    		ImGui::Checkbox("Demo Window", &m_showDemoWindow);
    		ImGui::Checkbox("Another Window", &m_showAnotherWindow);

    		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    		ImGui::ColorEdit3("Color", clearColor);

    		if (ImGui::Button("Button"))
    			++counter;
    		ImGui::SameLine();
    		ImGui::Text("Counter = %d", counter);

    		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    		ImGui::End();
	    }

    	if (m_showAnotherWindow)
    	{
    		ImGui::Begin("Another Window", &m_showAnotherWindow);
    		ImGui::Text("Hello from another window!");
    		if (ImGui::Button("Close Me"))
    			m_showAnotherWindow = false;
    		ImGui::End();
    	}

    	ImGui::Render();

    	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    	glClear(GL_COLOR_BUFFER_BIT);

    	glUseProgram(m_shaderProgram);
    	glBindVertexArray(m_VAO);
    	glDrawArrays(GL_TRIANGLES, 0, 3);

    	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    	ImGui::UpdatePlatformWindows();
    	ImGui::RenderPlatformWindowsDefault();

    	// 从ImGui的egl surface切换到主窗口的egl surface
    	eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);
    	eglSwapBuffers(m_eglDisplay, m_eglSurface);
    }

	using Win32Proc = LRESULT (WINAPI*)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static Win32Proc g_prevWndProc = nullptr;
	static LRESULT ImGuiWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		assert(g_prevWndProc != nullptr);

		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
			return true;

		return g_prevWndProc(hwnd, msg, wParam, lParam);
	}

	struct EGLData
	{
		EGLDisplay display;
		EGLContext context;
		EGLSurface surface;
	};

    void RenderModule::PlugInVideoDisplay(const IVideoDisplay& display)
    {
        // 初始化egl
        // 1. CreateEGLDisplay
        m_eglDisplay = eglGetDisplay(display.GetContext());
        if (m_eglDisplay == EGL_NO_DISPLAY)
            m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (m_eglDisplay == EGL_NO_DISPLAY)
        	throw BaseException("Failed to get an EGLDisplay");

        EGLint eglMajorVersion = 0;
        EGLint eglMinorVersion = 0;
        if (eglInitialize(m_eglDisplay, &eglMajorVersion, &eglMinorVersion) != EGL_TRUE)
        	throw EGLException("Failed to initialize EGLDisplay");

    	const char* version = eglQueryString(m_eglDisplay, EGL_VERSION);
    	spdlog::info("EGL version: {}", version);

    	const char* extensions = eglQueryString(m_eglDisplay, EGL_EXTENSIONS);
    	std::regex whiteRex("\\s");
    	std::string outputExtensions = std::regex_replace(extensions, whiteRex, "\n");
    	spdlog::info("EGL extensions:\n{}", outputExtensions);

        if (eglBindAPI(EGL_OPENGL_ES_API) != EGL_TRUE)
        	throw EGLException("Failed to bind EGL_OPENGL_ES_API");

        // 2. ChooseEGLConfig
    	const EGLint configurationAttributes[] = {
        	EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
			EGL_NONE
		};
        EGLint numConfigs = 0;
    	EGLConfig eglConfig;
        if (eglChooseConfig(m_eglDisplay, configurationAttributes, &eglConfig, 1, &numConfigs) != GL_TRUE)
        	throw EGLException("Failed to choose EGLConfig");

        // 3. Create EGL Surface
        m_eglSurface = eglCreateWindowSurface(m_eglDisplay, eglConfig, display.GetHandle(), nullptr);
        if (m_eglSurface == EGL_NO_SURFACE)
        	throw EGLException("Failed to create EGLSurface");

        // 4. Setup EGL context
    	const EGLint contextAttributes[] = {
        	EGL_CONTEXT_MAJOR_VERSION, 3,
			EGL_CONTEXT_MINOR_VERSION, 2,
    		EGL_CONTEXT_FLAGS_KHR, EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR,
			EGL_NONE
		};
        m_eglContext = eglCreateContext(m_eglDisplay, eglConfig, EGL_NO_CONTEXT, contextAttributes);
        if (m_eglContext == EGL_NO_CONTEXT)
        	throw EGLException("Failed to create EGLContext");

        if (eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext) != EGL_TRUE)
        	throw EGLException("Failed to make EGL current");

    	// 开启垂直同步
    	if (eglSwapInterval(m_eglDisplay, 1) != EGL_TRUE)
    		throw EGLException("Failed to set EGL swap interval");

    	// 查看opengl es版本
    	auto glVersion = reinterpret_cast<char const*>(glGetString(GL_VERSION));
    	spdlog::info(glVersion);

    	// 窗口程序插入ImGui处理输入事件的代码
    	g_prevWndProc = reinterpret_cast<Win32Proc>(GetWindowLongPtrW(display.GetHandle(), GWLP_WNDPROC));
    	SetWindowLongPtrW(display.GetHandle(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(ImGuiWindowProc));

    	// 初始化IMGUI
    	IMGUI_CHECKVERSION();
    	ImGui::CreateContext();
    	ImGuiIO& io = ImGui::GetIO();
    	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    	ImGui::StyleColorsDark();

    	ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 0.0f;
    	style.Colors[ImGuiCol_WindowBg].w = 1.0f;

    	ImGui_ImplWin32_InitForOpenGL(display.GetHandle());
    	ImGui_ImplOpenGL3_Init();

    	ImGuiPlatformIO& platformIO = ImGui::GetPlatformIO();
		assert(platformIO.Renderer_CreateWindow == nullptr);
    	assert(platformIO.Renderer_DestroyWindow == nullptr);
    	assert(platformIO.Renderer_SwapBuffers == nullptr);
    	assert(platformIO.Platform_RenderWindow == nullptr);
    	platformIO.Renderer_CreateWindow = [](ImGuiViewport* viewport) {
    		assert(viewport->RendererUserData == nullptr);

    		auto* data = TG_NEW EGLData;
    		data->display = eglGetCurrentDisplay();
    		data->context = eglGetCurrentContext();

    		// ChooseEGLConfig
    		const EGLint configurationAttributes[] = {
    			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
				EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
				EGL_NONE
			};
    		EGLint numConfigs;
			EGLConfig eglConfig;
    		if (eglChooseConfig(data->display, configurationAttributes, &eglConfig, 1, &numConfigs) != GL_TRUE)
    		{
    			spdlog::error("eglChooseConfig failed: {:#x}", eglGetError());
    			return;
    		}
    		if (numConfigs != 1)
    		{
    			spdlog::error("eglChooseConfig return no config");
    			return;
    		}
    		// 创建EGLSurface
    		data->surface = eglCreateWindowSurface(data->display, eglConfig,
				static_cast<HWND>(viewport->PlatformHandle), nullptr);
    		if (data->surface == EGL_NO_SURFACE)
    		{
    			spdlog::error("Failed to create EGL surface: {:#x}", eglGetError());
    			return;
    		}

    		viewport->RendererUserData = data;
    	};
    	platformIO.Renderer_DestroyWindow = [](ImGuiViewport* viewport) {
    		if (viewport->RendererUserData != nullptr)
    		{
    			auto* data = static_cast<EGLData *>(viewport->RendererUserData);
    			eglDestroySurface(data->display, data->surface);
    			delete data;
    			viewport->RendererUserData = nullptr;
    		}
    	};
    	platformIO.Renderer_SwapBuffers = [](ImGuiViewport* viewport, void*) {
    		if (auto* data = static_cast<EGLData*>(viewport->RendererUserData))
    			eglSwapBuffers(data->display, data->surface);
    	};
    	platformIO.Platform_RenderWindow = [](ImGuiViewport* viewport, void*) {
    		if (auto* data = static_cast<EGLData*>(viewport->RendererUserData))
    			eglMakeCurrent(data->display, data->surface, data->surface, data->context);
    	};

    	// 初始化三角形数据
    	InitialTriangle();
    }

	void RenderModule::InitialTriangle()
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
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
}
