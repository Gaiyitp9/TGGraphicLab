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
    	glDeleteBuffers(1, &m_EBO);
    	glDeleteProgram(m_shaderProgram);

    	if (m_getDisplay && eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT) == EGL_FALSE)
			Log::Instance().Error(EGLException("Failed to make EGL context current").what());
    	if (m_createSurface && eglDestroySurface(m_eglDisplay, m_eglSurface) == EGL_FALSE)
    		Log::Instance().Error(EGLException("Failed to destroy EGL surface").what());
    	if (m_createContext && eglDestroyContext(m_eglDisplay, m_eglContext) == EGL_FALSE)
    		Log::Instance().Error(EGLException("Failed to destroy EGL context").what());
    	if (m_getDisplay && eglTerminate(m_eglDisplay) == EGL_FALSE)
    		Log::Instance().Error(EGLException("Failed to terminate EGL display").what());
    }

	void RenderModule::PreUpdate()
	{

	}

    void RenderModule::Update()
    {
    	float clearColor[4]{ 0.2f, 0.3f, 0.3f, 1.0f };

    	if (eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext) == EGL_FALSE)
    		throw EGLException("Failed to make EGL current");

    	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    	glClear(GL_COLOR_BUFFER_BIT);

    	glUseProgram(m_shaderProgram);
    	glBindVertexArray(m_VAO);
    	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    	glBindVertexArray(0);
    }

	void RenderModule::PostUpdate()
	{
    	eglSwapBuffers(m_eglDisplay, m_eglSurface);
	}

    void RenderModule::ConnectToVideoDisplay(const IVideoDisplay& display)
    {
    	// 绑定OpenGL ES
    	if (eglBindAPI(EGL_OPENGL_ES_API) == EGL_FALSE)
    		throw EGLException("Failed to bind OpenGL ES API");

        // 创建EGLDisplay并初始化
        m_eglDisplay = eglGetDisplay(display.GetContext());
        if (m_eglDisplay == EGL_NO_DISPLAY)
            m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (m_eglDisplay == EGL_NO_DISPLAY)
        	throw BaseException("No display connection matching native_display is available");

        EGLint eglMajorVersion = 0;
        EGLint eglMinorVersion = 0;
        if (eglInitialize(m_eglDisplay, &eglMajorVersion, &eglMinorVersion) == EGL_FALSE)
        	throw EGLException("Failed to initialize EGLDisplay");
    	m_getDisplay = true;

    	const char* version = eglQueryString(m_eglDisplay, EGL_VERSION);
    	if (version == nullptr)
    		throw EGLException("Failed to query EGL_VERSION");
    	Log::Instance().Info("EGL version: {}", version);

    	const char* extensions = eglQueryString(m_eglDisplay, EGL_EXTENSIONS);
    	if (extensions == nullptr)
    		throw EGLException("Failed to query EGL_EXTENSIONS");
    	std::regex whiteRex("\\s");
    	std::string outputExtensions = std::regex_replace(extensions, whiteRex, "\n");
    	Log::Instance().Info("EGL extensions:\n{}", outputExtensions);

        // 创建EGL Surface
    	const EGLint configurationAttributes[] = {
        	EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
			EGL_NONE
		};
        EGLint numConfigs = 0;
    	EGLConfig eglConfig;
        if (eglChooseConfig(m_eglDisplay, configurationAttributes, &eglConfig, 1, &numConfigs) == EGL_FALSE)
        	throw EGLException("Failed to choose EGLConfig");

        m_eglSurface = eglCreateWindowSurface(m_eglDisplay, eglConfig, display.GetHandle(), nullptr);
        if (m_eglSurface == EGL_NO_SURFACE)
        	throw EGLException("Failed to create EGLSurface");
    	m_createSurface = true;

        // 创建EGL context
    	const EGLint contextAttributes[] = {
        	EGL_CONTEXT_MAJOR_VERSION, 3,
			EGL_CONTEXT_MINOR_VERSION, 2,
    		EGL_CONTEXT_FLAGS_KHR, EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR,
			EGL_NONE
		};
        m_eglContext = eglCreateContext(m_eglDisplay, eglConfig, EGL_NO_CONTEXT, contextAttributes);
        if (m_eglContext == EGL_NO_CONTEXT)
        	throw EGLException("Failed to create EGLContext");
    	m_createContext = true;

    	// 开启垂直同步
    	if (eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext) == EGL_FALSE)
    		throw EGLException("Failed to make EGL current");
    	if (eglSwapInterval(m_eglDisplay, 1) == EGL_FALSE)
    		throw EGLException("Failed to set EGL swap interval");

    	// 查询OpenGL ES相关信息
    	auto glVersion = reinterpret_cast<char const*>(glGetString(GL_VERSION));
    	if (glVersion == nullptr)
    		throw OpenGLException("Failed to get OpenGL ES version");
    	Log::Instance().Info("OpenGL version: {}", glVersion);
    	auto glVendor = reinterpret_cast<char const*>(glGetString(GL_VENDOR));
    	if (glVendor == nullptr)
    		throw OpenGLException("Failed to get OpenGL ES vendor");
    	Log::Instance().Info("Company for the OpenGL implementation: {} ", glVendor);
    	auto glRenderer = reinterpret_cast<char const*>(glGetString(GL_RENDERER));
    	if (glRenderer == nullptr)
    		throw OpenGLException("Failed to get OpenGL ES renderer");
    	Log::Instance().Info("Name of the OpenGL renderer: {}", glRenderer);
    	auto glShadingLanguageVersion = reinterpret_cast<char const*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
    	if (glShadingLanguageVersion == nullptr)
    		throw OpenGLException("Failed to get OpenGL ES shading language version");
    	Log::Instance().Info("Shading language version: {}", glShadingLanguageVersion);

    	// 初始化三角形数据
    	InitialTriangle();
    }

	void RenderModule::InitialTriangle()
    {
        float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
        	-0.5f,  0.5f, 0.0f,
		};

    	unsigned int indices[] = {
    		0, 1, 2,
    		2, 3, 0,
    	};

		glGenBuffers(1, &m_VBO);
    	glGenBuffers(1, &m_EBO);
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
		// 把顶点数据传入显存
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		std::ifstream vertexFile("../../Shaders/GLSL/simple.vert");
		if (!vertexFile)
			throw BaseException("Failed to load simple vertex shader source file");

		std::ostringstream vertexBuffer;
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

    	std::ifstream geometryFile("../../Shaders/GLSL/wireframe.geom");
    	if (!geometryFile)
    		throw BaseException("Failed to load wireframe shader source file");
    	std::ostringstream geometryBuffer;
    	geometryBuffer << geometryFile.rdbuf();
    	std::string geometryStr = geometryBuffer.str();
    	char const* geometryShaderSource = geometryStr.c_str();
    	unsigned int geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    	glShaderSource(geometryShader, 1, &geometryShaderSource, nullptr);
    	glCompileShader(geometryShader);
    	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
    	if (!success)
    	{
    		glGetShaderInfoLog(geometryShader, 512, nullptr, infoLog);
    		Log::Instance().Error("ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n{}", infoLog);
    	}

		std::ifstream fragmentFile("../../Shaders/GLSL/simple.frag");
		if (!fragmentFile)
			throw BaseException("Failed to load simple fragment shader source file");

		std::ostringstream fragmentBuffer;
		fragmentBuffer << fragmentFile.rdbuf();
		std::string fragStr = fragmentBuffer.str();
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
    	if (m_wireframe)
    		glAttachShader(m_shaderProgram, geometryShader);
		glAttachShader(m_shaderProgram, fragmentShader);
		glLinkProgram(m_shaderProgram);
		glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_shaderProgram, 512, nullptr, infoLog);
			Log::Instance().Error("ERROR::SHADER::PROGRAM::LINKING_FAILED\n {}", infoLog);
		}
		glDeleteShader(vertexShader);
    	glDeleteShader(geometryShader);
		glDeleteShader(fragmentShader);
    }
}
