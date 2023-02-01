/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Window/MainWindow.hpp"
#include "Window/PopupWindow.hpp"
#include "Chronometer.hpp"
#include "UnitTest.hpp"
#include <locale>
#include <unordered_map>
#include "GraphicsLayer.hpp"

namespace TG
{
	class Application
	{
	public:
		Application();
		~Application();
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		int Run();

	private:
		std::locale locale;						// 语言区域
		std::shared_ptr<Chronometer> timer;		// 高精度计时器
		std::unordered_map<std::wstring, std::shared_ptr<Window>> windows;						// 窗口集合
		std::unique_ptr<Graphics::GraphicsLayer<Graphics::DeviceType::DirectX11>> d3d11Layer;	// 渲染层

		int screenWidth;		// 主显示器的宽
		int screenHeight;		// 主显示器的高

	private:
		UnitTest unitTest;		// 单元测试类
	};
}