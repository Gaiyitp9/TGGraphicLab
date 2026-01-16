/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Application.h"
#include <ranges>

namespace TG
{
	Application::Application()
	{
	    m_platformModule = std::make_unique<PlatformModule>();
	    std::unique_ptr<RenderModule> renderModule = std::make_unique<RenderModule>();
	    std::unique_ptr<EditorModule> editorModule = std::make_unique<EditorModule>();

		// 初始化渲染模块，接入视频接口和计时器接口
		renderModule->SetupRenderer(m_platformModule->GetVideoPort(), m_platformModule->GetTimer());
		renderModule->Subscribe(m_platformModule->onWindowResize);
		// 编辑器模块设置渲染器，并接入视频接口
		editorModule->SetRenderer(renderModule->GetRenderer());
		editorModule->PlugInVideoPort(m_platformModule->GetVideoPort());

		m_modules.emplace_back(std::move(renderModule));
		m_modules.emplace_back(std::move(editorModule));

		// std::string_view mbstr = "z\u00df\u6c34\U0001f34c";
		// std::wstring_view wcstr = L"z\u00df\u6c34\U0001f34c";
	}

	Application::~Application() = default;

	int Application::Run()
	{
		while (true)
		{
		    m_platformModule->Update();
			if (m_platformModule->ShouldExit())
				return m_platformModule->ExitCode();

			for (const auto& module : m_modules)
				module->Update();

			for (const auto& module : m_modules | std::views::reverse)
				module->PostUpdate();
		}
	}
}
