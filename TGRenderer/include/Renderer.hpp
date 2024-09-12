/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Editor/MainWindow.h"
#include "Chronometer.h"
#include "Input/Manager.hpp"
#include "Input/Mouse.h"
#include "Input/Keyboard.h"
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
		// Create and Initialize an EGLDisplay from the native display
		bool CreateEGLDisplay();
		bool ChooseEGLConfig();
		bool CreateEGLSurface();
		bool SetupEGLContext();
		void InitialTriangle();

		int m_screenWidth;			// 主显示器的尺寸
		int m_screenHeight;
		int m_windowWidth = 800;	// 主窗口的尺寸
		int m_windowHeight = 600;
		MainWindow m_mainWindow;	// 主窗口

        Chronometer m_timer;		// 高精度计时器
        Input::Manager<Input::Mouse, Input::Keyboard> m_input;	// 输入管理器，使用鼠标和键盘输入

		EGLDisplay m_eglDisplay{};
		EGLConfig m_eglConfig{};
		EGLSurface m_eglSurface{};
		EGLContext m_eglContext{};

		GLuint m_shaderProgram{};
		GLuint m_VAO{};
		GLuint m_VBO{};
	};
}
