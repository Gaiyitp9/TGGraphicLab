/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include <fstream>
#include <regex>
#include "Modules/RenderModule.h"
#include "Exception/EGLException.h"
#include "Exception/OpenGLException.h"
#include "Diagnostic/Log.h"

namespace TG
{
    RenderModule::RenderModule() = default;
    RenderModule::~RenderModule()
    {
    	glDeleteVertexArrays(1, &m_VAO);
    	glDeleteBuffers(1, &m_VBO);
    	glDeleteProgram(m_shaderProgram);

    	eglDestroySurface(m_eglDisplay, m_eglSurface);
    	eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    	eglDestroyContext(m_eglDisplay, m_eglContext);
    	eglTerminate(m_eglDisplay);
    }

	void RenderModule::PreUpdate()
	{

	}

    void RenderModule::Update()
    {
    	float clearColor[4]{ 0.2f, 0.3f, 0.3f, 1.0f };

    	// 从ImGui的egl surface切换到主窗口的egl surface
    	eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);

    	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    	glClear(GL_COLOR_BUFFER_BIT);

    	glUseProgram(m_shaderProgram);
    	glBindVertexArray(m_VAO);
    	glDrawArrays(GL_TRIANGLES, 0, 3);
    }

	void RenderModule::PostUpdate()
	{
    	eglSwapBuffers(m_eglDisplay, m_eglSurface);
	}

    void RenderModule::PlugInVideoDisplay(const IVideoDisplay& display)
    {
        // 初始化egl
        // 1. CreateEGLDisplay
        m_eglDisplay = eglGetDisplay(display.GetContext());
        if (m_eglDisplay == EGL_NO_DISPLAY)
            m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (m_eglDisplay == EGL_NO_DISPLAY)
        	throw BaseException("No display connection matching native_display is available");

        EGLint eglMajorVersion = 0;
        EGLint eglMinorVersion = 0;
        if (eglInitialize(m_eglDisplay, &eglMajorVersion, &eglMinorVersion) != EGL_TRUE)
        	throw EGLException("Failed to initialize EGLDisplay");

    	const char* version = eglQueryString(m_eglDisplay, EGL_VERSION);
    	if (!version)
    		throw EGLException("Failed to query EGL_VERSION");
    	Log::Instance().Info("EGL version: {}", version);

    	const char* extensions = eglQueryString(m_eglDisplay, EGL_EXTENSIONS);
    	if (!extensions)
    		throw EGLException("Failed to query EGL_EXTENSIONS");
    	std::regex whiteRex("\\s");
    	std::string outputExtensions = std::regex_replace(extensions, whiteRex, "\n");
    	Log::Instance().Info("EGL extensions:\n{}", outputExtensions);

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
    	if (!glVersion)
    		throw OpenGLException("Failed to get glGetString");
    	Log::Instance().Info("OpenGL version: {}", glVersion);
    	auto glVendor = reinterpret_cast<char const*>(glGetString(GL_VENDOR));
    	if (!glVendor)
    		throw OpenGLException("Failed to get glGetString");
    	Log::Instance().Info("Company for the OpenGL implementation: {} ", glVendor);
    	auto glRenderer = reinterpret_cast<char const*>(glGetString(GL_RENDERER));
    	if (!glRenderer)
    		throw OpenGLException("Failed to get glGetString");
    	Log::Instance().Info("Name of the OpenGL renderer: {}", glRenderer);
    	auto glShadingLanguageVersion = reinterpret_cast<char const*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
    	if (!glShadingLanguageVersion)
    		throw OpenGLException("Failed to get glGetString");
    	Log::Instance().Info("Shading language version: {}", glShadingLanguageVersion);

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
			Log::Instance().Error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}", infoLog);
		}

		std::ifstream fragmentFile("../../Shaders/GLSL/simple.frag");
		if (!fragmentFile)
		{
			Log::Instance().Error("Load simple fragment file failed!");
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
			Log::Instance().Error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n {}", infoLog);
		}

		m_shaderProgram = glCreateProgram();
		glAttachShader(m_shaderProgram, vertexShader);
		glAttachShader(m_shaderProgram, fragmentShader);
		glLinkProgram(m_shaderProgram);
		glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_shaderProgram, 512, nullptr, infoLog);
			Log::Instance().Error("ERROR::SHADER::PROGRAM::LINKING_FAILED\n {}", infoLog);
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
    }
}
