/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "SlimWindows.h"
#include "Application.h"
#include <exception>
#include <iostream>

int CALLBACK wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ PWSTR     lpCmdLine,
	_In_ int       nShowCmd)
{
	LCH::Application app;
	try
	{
		return app.Run();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return -1;
	}
}