/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "SlimWindows.h"
#include "Application.hpp"
#include "Diagnostics/Debug.hpp"
#include <tchar.h>

int CALLBACK _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR     lpCmdLine,
	_In_ int       nShowCmd)
{
	try
	{
		TG::Application app;
		return app.Run();
	}
	catch (const TG::BaseException& e)
	{
		TG::Debug::Log(e.what());
	}
	catch (const std::exception& e)
	{
		TG::Debug::LogLine("Standard exception: ");
		TG::Debug::Log(e.what());
	}
	catch (...)
	{
		TG::Debug::Log(L"Unknown Exception");
	}

	return -1;
}