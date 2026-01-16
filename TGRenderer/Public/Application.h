/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Modules/PlatformModule.h"
#include "Modules/RenderModule.h"
#include "Modules/EditorModule.h"
#include <vector>

namespace TG
{
	class Application
	{
	public:
		Application();
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;
		~Application();

		int Run();

	private:
		std::unique_ptr<PlatformModule> m_platformModule;
		std::vector<std::unique_ptr<Module>> m_modules;
	};
}
