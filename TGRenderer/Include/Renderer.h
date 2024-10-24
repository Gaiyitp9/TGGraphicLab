/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Modules/PlatformModule.h"
#include "Modules/InputModule.h"
#include "Modules/EditorModule.h"
#include "DynamicGles.h"

namespace TG
{
	class Renderer
	{
	public:
		Renderer();
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(Renderer&&) = delete;
		~Renderer();

		int Run();

	private:
		bool CreateEGLDisplay();
		bool ChooseEGLConfig();
		bool CreateEGLSurface();
		bool SetupEGLContext();
		void InitialTriangle();

		EGLDisplay m_eglDisplay{};
		EGLConfig m_eglConfig{};
		EGLSurface m_eglSurface{};
		EGLContext m_eglContext{};

		GLuint m_shaderProgram{};
		GLuint m_VAO{};
		GLuint m_VBO{};

		PlatformModule m_platformModule;
		InputModule m_inputModule;
		// EditorModule m_editorModule;
	};
}
