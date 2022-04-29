/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "SlimWindows.h"
#include "Application.h"
#include "Diagnostics/Debug.h"
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
		LCH::Debug::Log(e.what());
	}
	catch (const std::exception& e)
	{
		LCH::Debug::LogLine("Standard exception: ");
		LCH::Debug::Log(e.what());
	}
	catch (...)
	{
		LCH::Debug::Log(L"Unknown Exception");
	}

	return -1;
}