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
	class Renderer
	{
	public:
		Renderer();
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(Renderer&&) = delete;
		~Renderer();

		int Run();

	private:
		std::shared_ptr<PlatformModule> m_platformModule;
		std::shared_ptr<InputModule> m_inputModule;
		std::shared_ptr<RenderModule> m_renderModule;
		std::shared_ptr<EditorModule> m_editorModule;
		std::vector<std::shared_ptr<Module>> m_modules;
	};
}
