/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Modules/PlatformModule.h"
#include "Modules/InputModule.h"
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
		std::shared_ptr<PlatformModule> m_platformModule;
		std::shared_ptr<InputModule> m_inputModule;
		std::shared_ptr<RenderModule> m_renderModule;
		std::shared_ptr<EditorModule> m_editorModule;
		std::vector<std::shared_ptr<Module>> m_modules;
	};
}
