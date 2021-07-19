/****************************************************************
* Copyright (c) IGOTGAMES(IGG ShenZhen). All rights reserved.	*
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
#include <iostream>

int WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd)
{
	IGGSZLab::Window wnd1(800, 600);
	std::chrono::system_clock c;
	std::wstringstream str;
	const std::chrono::time_zone* z = std::chrono::current_zone();
	z->to_local(c.now());
	str << z->to_local(c.now());
	OutputDebugString(str.str().c_str());

	while (true)
	{
		if (const auto code = IGGSZLab::Window::ProcessMessage())
			return *code;
	}
}