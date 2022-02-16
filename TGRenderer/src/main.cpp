/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "SlimWindows.h"
#include <tchar.h>
#include "Application.h"
#include <exception>
#include <iostream>

int CALLBACK _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ PTSTR     lpCmdLine,
	_In_ int       nShowCmd)
{
	LCH::Application app;
	return app.Run();
}