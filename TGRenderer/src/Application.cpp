/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Application.h"
#include "Window.h"
#include "WindowRegister.h"
#include "TimeSystem.h"
#include "Utility.h"
#include "../resource.h"
#include <iostream>
#include <format>

namespace LCH
{
	Application::Application()
	{
		WindowRegister::GetInstance()->Initialize({IDI_ICON1, IDI_ICON2});
	}

	Application::~Application()
	{

	}

	int Application::Run()
	{
		LCH::Window wnd1(800, 600, L"TG Renderer");
		LCH::Window wnd2(400, 300, L"child window", wnd1.GetHwnd());
		LCH::TimeSystem timer;

		std::wcout << timer.Time() << std::endl;
		std::string time = Utility::WideStringToAnsi(timer.Time());
		std::cout << time << std::endl;


		timer.Tick();
		int i = 1000000;
		while (i-- > 0);
		timer.Tick();

		std::wcout << timer.DeltaTime() << std::endl;

		while (true)
		{
			if (const auto code = LCH::Window::ProcessMessage())
			{
				return *code;
			}
		}
	}
}