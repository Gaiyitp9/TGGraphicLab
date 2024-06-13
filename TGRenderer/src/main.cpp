/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Renderer.hpp"
#include "PAL/Windows/Win32API.h"
#include "spdlog/spdlog.h"

int CALLBACK wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR     lpCmdLine,
                      _In_ int       nShowCmd)
{
	try
	{
		TG::Renderer app;
		return app.Run();
	}
	catch (const std::exception& e)
	{
		spdlog::error(e.what());
	}
	catch (...)
	{
		spdlog::error("Unknown Exception");
	}

	return -1;
}
