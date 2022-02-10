/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "pch.h"
#include "Window.h"
#include "TimeSystem.h"
#include "../resource.h"
#include <iostream>
#include <format>

int CALLBACK _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ PTSTR     lpCmdLine,
	_In_ int       nShowCmd)
{
	LCH::Window wnd1(800, 600, IDI_ICON1);
	LCH::TimeSystem timer;

	std::wcout << std::format(L"test{:#x}\n", 120);

	std::wcout << timer.Time() << std::endl;

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