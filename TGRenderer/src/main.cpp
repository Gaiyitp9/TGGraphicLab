/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Renderer.hpp"

int CALLBACK wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR     lpCmdLine,
	_In_ int       nShowCmd)
{
	try
	{
		TG::Renderer app;
        OutputDebugStringA("test output\n");
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
		TG::Debug::Log("Unknown Exception");
	}

	return -1;
}
