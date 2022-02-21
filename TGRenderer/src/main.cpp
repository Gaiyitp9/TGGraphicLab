/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "SlimWindows.h"
#include "Application.h"
#include "Diagnostics/BaseException.h"
#include <exception>
#include <iostream>
#include <tchar.h>

int CALLBACK _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR     lpCmdLine,
	_In_ int       nShowCmd)
{
	LCH::Application app;
	try
	{
		return app.Run();
	}
	catch (const LCH::BaseException& e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "Standard exception:\n" << e.what() << std::endl;
	}
	catch (...)
	{
		std::wcout << L"Unknown Exception" << std::endl;
	}
	return -1;
}