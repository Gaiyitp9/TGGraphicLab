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
		void InitialOpenGLES();
		void InitTriangle();

		int m_screenWidth;			// 主显示器的尺寸
		int m_screenHeight;
		int m_windowWidth = 800;	// 主窗口的尺寸
		int m_windowHeight = 600;
		MainWindow m_mainWindow;	// 主窗口

        Chronometer m_timer;		// 高精度计时器
        Input::Manager<Input::Mouse, Input::Keyboard> m_input;	// 输入管理器，使用鼠标和键盘输入

		PIXELFORMATDESCRIPTOR m_pfd;
		HGLRC m_hglrc;

		unsigned int m_shaderProgram{};
		unsigned int m_VAO{};
		unsigned int m_VBO{};
	};
}