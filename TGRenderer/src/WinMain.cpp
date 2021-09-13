/****************************************************************
* Copyright (c) LiaoChenhan. All rights reserved.				*
*																*
* This code is licensed under the MIT License (MIT).			*
* THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF				*
* ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY			*
* IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR				*
* PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.				*
*																*
* Author: LiaoChenhan											*
*****************************************************************/
#include "pch.h"
#include "Window.h"
#include "TimeSystem.h"
#include <iostream>

int WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd)
{
	LCH::Window wnd1(800, 600);
	LCH::TimeSystem timer;
	OutputDebugString(timer.Time().c_str());
	timer.Tick();
	int i = 100000;
	while (i-- > 0);
	timer.Tick();
	OutputDebugString(L"\n");
	OutputDebugString(std::to_wstring(timer.DeltaTime()).c_str());

	while (true)
	{
		if (const auto code = LCH::Window::ProcessMessage())
		{
			return *code;
		}
	}
}