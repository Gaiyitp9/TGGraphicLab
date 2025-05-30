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
		m_platformModule = std::make_shared<PlatformModule>();
		m_inputModule = std::make_shared<InputModule>();
		m_renderModule = std::make_shared<RenderModule>();
		// m_editorModule  = std::make_shared<EditorModule>();

		// 输入模块订阅输入事件委托
		m_inputModule->Subscribe(m_platformModule->onMouseEvent);
		m_inputModule->Subscribe(m_platformModule->onKeyboardEvent);
		// 渲染模块接入视频接口
		m_renderModule->PlugInVideoPort(m_platformModule->GetWindow(), m_platformModule->GetTimer());
		m_renderModule->Subscribe(m_platformModule->onWindowResize);
		// 编辑器模块设置渲染器，并接入视频接口
		// m_editorModule->SetRenderer(m_renderModule->GetRenderer());
		// m_editorModule->PlugInVideoPort(m_platformModule->GetWindow());

		m_modules.emplace_back(m_platformModule);
		m_modules.emplace_back(m_inputModule);
		m_modules.emplace_back(m_renderModule);
		// m_modules.emplace_back(m_editorModule);

		// std::string_view mbstr = "z\u00df\u6c34\U0001f34c";
		// std::wstring_view wcstr = L"z\u00df\u6c34\U0001f34c";
	}

	Application::~Application() = default;

	int Application::Run()
	{
		while (true)
		{
			if (m_platformModule->ShouldExit())
				return m_platformModule->ExitCode();

			for (const auto& module : m_modules)
				module->Update();

			for (const auto& module : m_modules | std::views::reverse)
				module->PostUpdate();
		}
	}
}
