/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Renderer.h"
#include "spdlog/spdlog.h"

int CALLBACK main()
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
