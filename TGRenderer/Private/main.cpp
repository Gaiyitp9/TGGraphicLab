/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Application.h"
#include "Diagnostic/Log.hpp"
#include "mimalloc.h"

int CALLBACK main()
{
	mi_version();
	try
	{
		TG::Application app;
		return app.Run();
	}
	catch (const std::exception& e)
	{
		TG::LogError(e.what());
	}
	catch (...)
	{
		TG::LogError("Unknown Exception");
	}

	return -1;
}
