/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Renderer.h"
#include "spdlog/spdlog.h"

namespace TG
{
	Renderer::Renderer()
	{
		// 输入模块监听输入事件
		m_platformModule.AddEventListener(m_inputModule.GetMouseEventHandler());
		m_platformModule.AddEventListener(m_inputModule.GetKeyboardEventHandler());
		// 渲染模块接入窗口
		m_renderModule.PlugInVideoDisplay(m_platformModule.GetWindow());

		// std::string_view mbstr = "z\u00df\u6c34\U0001f34c";
		// std::wstring_view wcstr = L"z\u00df\u6c34\U0001f34c";
	}

	Renderer::~Renderer() = default;

	int Renderer::Run()
	{
		while (true)
		{
			m_inputModule.Update();
			m_platformModule.Update();
			if (m_platformModule.ShouldExit())
				return m_platformModule.ExitCode();

            if (m_inputModule.GetKeyUp(Input::KeyCode::Space))
                spdlog::info("space up");

			m_renderModule.Update();

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
