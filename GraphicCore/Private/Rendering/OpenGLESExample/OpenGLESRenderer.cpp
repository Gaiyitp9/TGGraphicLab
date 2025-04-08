/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Rendering/OpenGLESExample/OpenGLESRenderer.h"
#include "Exception/EGLException.h"
#include "Exception/OpenGLException.h"
#include "Diagnostic/Log.hpp"
#include "QuadExample.h"
#include <regex>

namespace TG
{
    OpenGLESRenderer::OpenGLESRenderer(HWND handle, HDC context)
    {
		// 绑定OpenGL ES
    	if (eglBindAPI(EGL_OPENGL_ES_API) == EGL_FALSE)
    		throw EGLException::Create("Failed to bind OpenGL ES API");

        // 创建EGLDisplay并初始化
        m_eglDisplay = eglGetDisplay(context);
        if (m_eglDisplay == EGL_NO_DISPLAY)
            m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (m_eglDisplay == EGL_NO_DISPLAY)
        	throw BaseException::Create("No display connection matching native_display is available");

        EGLint eglMajorVersion = 0;
        EGLint eglMinorVersion = 0;
        if (eglInitialize(m_eglDisplay, &eglMajorVersion, &eglMinorVersion) == EGL_FALSE)
        	throw EGLException::Create("Failed to initialize EGLDisplay");
    	m_getDisplay = true;

    	const char* version = eglQueryString(m_eglDisplay, EGL_VERSION);
    	if (version == nullptr)
    		throw EGLException::Create("Failed to query EGL_VERSION");
    	LogInfo("EGL version: {}", version);

    	const char* extensions = eglQueryString(m_eglDisplay, EGL_EXTENSIONS);
    	if (extensions == nullptr)
    		throw EGLException::Create("Failed to query EGL_EXTENSIONS");
    	std::regex whiteRex("\\s");
    	std::string outputExtensions = std::regex_replace(extensions, whiteRex, "\n");
    	LogInfo("EGL extensions:\n{}", outputExtensions);

        // 创建EGL Surface
    	const EGLint configurationAttributes[] = {
        	EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
			EGL_NONE
		};
        EGLint numConfigs = 0;
    	EGLConfig eglConfig;
        if (eglChooseConfig(m_eglDisplay, configurationAttributes, &eglConfig, 1, &numConfigs) == EGL_FALSE)
        	throw EGLException::Create("Failed to choose EGLConfig");

        m_eglSurface = eglCreateWindowSurface(m_eglDisplay, eglConfig, handle, nullptr);
        if (m_eglSurface == EGL_NO_SURFACE)
        	throw EGLException::Create("Failed to create EGLSurface");
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
        	throw EGLException::Create("Failed to create EGLContext");
    	m_createContext = true;

    	if (eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext) == EGL_FALSE)
    		throw EGLException::Create("Failed to make EGL current");
    	// 开启垂直同步
    	if (eglSwapInterval(m_eglDisplay, 1) == EGL_FALSE)
    		throw EGLException::Create("Failed to set EGL swap interval");

    	// 查询OpenGL ES相关信息
    	auto glVersion = reinterpret_cast<char const*>(glGetString(GL_VERSION));
    	if (glVersion == nullptr)
    		throw OpenGLException::Create("Failed to get OpenGL ES version");
    	LogInfo("OpenGL version: {}", glVersion);
    	auto glVendor = reinterpret_cast<char const*>(glGetString(GL_VENDOR));
    	if (glVendor == nullptr)
    		throw OpenGLException::Create("Failed to get OpenGL ES vendor");
    	LogInfo("Company for the OpenGL implementation: {} ", glVendor);
    	auto glRenderer = reinterpret_cast<char const*>(glGetString(GL_RENDERER));
    	if (glRenderer == nullptr)
    		throw OpenGLException::Create("Failed to get OpenGL ES renderer");
    	LogInfo("Name of the OpenGL renderer: {}", glRenderer);
    	auto glShadingLanguageVersion = reinterpret_cast<char const*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
    	if (glShadingLanguageVersion == nullptr)
    		throw OpenGLException::Create("Failed to get OpenGL ES shading language version");
    	LogInfo("Shading language version: {}", glShadingLanguageVersion);
    	GLint maxAttribs = 0;
    	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);
    	LogInfo("Max vertex attributes: {}", maxAttribs);

    	// 正面朝向设置为顺时针
    	// glFrontFace(GL_CW);

    	m_sample = std::make_unique<QuadExample>();
    }

    OpenGLESRenderer::~OpenGLESRenderer()
    {
    	m_sample.reset();

    	if (m_getDisplay && eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT) == EGL_FALSE)
    		LogError(EGLException::Create("Failed to make EGL context current").what());
    	if (m_createSurface && eglDestroySurface(m_eglDisplay, m_eglSurface) == EGL_FALSE)
    		LogError(EGLException::Create("Failed to destroy EGL surface").what());
    	if (m_createContext && eglDestroyContext(m_eglDisplay, m_eglContext) == EGL_FALSE)
    		LogError(EGLException::Create("Failed to destroy EGL context").what());
    	if (m_getDisplay && eglTerminate(m_eglDisplay) == EGL_FALSE)
    		LogError(EGLException::Create("Failed to terminate EGL display").what());
    }

	void OpenGLESRenderer::Render()
	{
    	if (eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext) == EGL_FALSE)
    		throw EGLException::Create("Failed to make EGL current");

    	m_sample->Render();
	}

	void OpenGLESRenderer::Present()
	{
    	eglSwapBuffers(m_eglDisplay, m_eglSurface);
	}

	void OpenGLESRenderer::FrameBufferResizeCallback(unsigned int width, unsigned int height)
	{
		glViewport(0, 0, static_cast<int>(width), static_cast<int>(height));
	}
}
