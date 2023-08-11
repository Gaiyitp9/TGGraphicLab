/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Window/MainWindow.hpp"
#include "Window/PopupWindow.hpp"
#include "Input/Manager.hpp"
#include "Chronometer.hpp"
#include "UnitTest.hpp"
#include <locale>
#include <unordered_map>
#include "GraphicsLayer.hpp"
#include "Graphics/OpenGL/FactoryGL.hpp"
#include "glad/wgl.h"
#include "Input/Mouse.hpp"
#include "Input/Keyboard.hpp"

namespace TG
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		int Run();

	private:
		std::locale m_locale;               // 语言区域
		MainWindow m_mainWindow;            // 主窗口
        Chronometer m_timer;                // 高精度计时器
        Input::Manager<Input::Mouse, Input::Keyboard> m_input;           // 输入管理器，使用鼠标和键盘输入
		//std::unique_ptr<Graphics::GraphicsLayer> d3d11Layer;
        Graphics::FactoryGL m_factory;
        std::unique_ptr<Graphics::RenderDeviceGL> m_device;
        std::unique_ptr<Graphics::DeviceContextGL> m_context;

		int m_screenWidth;		            // 主显示器的宽
		int m_screenHeight;		            // 主显示器的高

		UnitTest m_unitTest;		        // 单元测试类
	};
}