/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Renderer.h"
#include "Diagnostic/Log.h"
#include "mimalloc.h"

int CALLBACK main()
{
	mi_version();
	try
	{
		TG::Renderer app;
		return app.Run();
	}
	catch (const std::exception& e)
	{
		TG::Log::Instance().Error(e.what());
	}
	catch (...)
	{
		TG::Log::Instance().Error("Unknown Exception");
	}

	return -1;
}
