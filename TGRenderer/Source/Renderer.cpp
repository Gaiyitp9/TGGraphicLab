/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Renderer.h"
#include "Diagnostic/Log.h"

namespace TG
{
	Renderer::Renderer()
	{
		m_platformModule = std::make_shared<PlatformModule>();
		m_inputModule = std::make_shared<InputModule>();
		m_renderModule = std::make_shared<RenderModule>();
		m_editorModule  = std::make_shared<EditorModule>();

		// 输入模块监听输入事件
		m_inputModule->Subscribe(m_platformModule->OnMouseEvent());
		m_inputModule->Subscribe(m_platformModule->OnKeyboardEvent());
		// 渲染模块接入窗口
		m_renderModule->ConnectToVideoDisplay(m_platformModule->GetWindow());
		// 编辑器模块接入窗口
		m_editorModule->ConnectToVideoPlay(m_platformModule->GetWindow());

		m_modules.emplace_back(m_platformModule);
		m_modules.emplace_back(m_inputModule);
		m_modules.emplace_back(m_renderModule);
		m_modules.emplace_back(m_editorModule);

		// std::string_view mbstr = "z\u00df\u6c34\U0001f34c";
		// std::wstring_view wcstr = L"z\u00df\u6c34\U0001f34c";
	}

	Renderer::~Renderer() = default;

	int Renderer::Run()
	{
		while (true)
		{
			for (const auto& module : m_modules)
				module->PreUpdate();

			if (m_platformModule->ShouldExit())
				return m_platformModule->ExitCode();

			for (const auto& module : m_modules)
				module->Update();

            if (m_inputModule->GetKeyUp(Input::KeyCode::Space))
	            Log::Instance().Info("space up");

			for (const auto& module : m_modules)
				module->PostUpdate();

			// const float c = sin(m_timer.TotalTime() * 0.001f) / 2.0f + 0.5f;
			//d3d11Layer->ClearBackground(Math::Color::AliceBlue * c);
			//d3d11Layer->Update();
//#ifdef _DEBUG
//			if (mainWnd->Input().GetKeyDown(KeyCode::Space))
//				d3d11Layer->dbgInfo->ReportLiveObjects();
//#endif
            // Math::Color color = Math::Color::AliceBlue * c;
            // glClearColor(color.r(), color.g(), color.b(), color.a());
            // glClear(GL_COLOR_BUFFER_BIT);
		}
	}
}
