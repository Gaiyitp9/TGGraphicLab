/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#pragma once

#include "Window.h"
#include "TimeSystem.h"
#include <locale>
#include <unordered_map>
#include <string>
#include <memory>

namespace LCH
{
	class Application
	{
	public:
		Application();
		~Application();
		Application(const Application&) = delete;
		const Application& operator=(const Application&) = delete;

		int Run();

	private:
		std::locale locale;		// 语言区域
		TimeSystem timer;		// 时间系统

		std::unordered_map<std::wstring, std::unique_ptr<Window>> windows;	// 所有窗口集合

		long count = 0;
	};
}