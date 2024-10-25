/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Renderer.h"
#include "spdlog/spdlog.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl3.h"

namespace TG
{
	Renderer::Renderer()
	{
		// 输入模块监听输入事件
		m_platformModule.AddEventListener(m_inputModule.GetMouseEventHandler());
		m_platformModule.AddEventListener(m_inputModule.GetKeyboardEventHandler());
		// 渲染模块接入窗口
		m_renderModule.PlugInVideoDisplay(m_platformModule.GetWindow());

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		ImGui::StyleColorsDark();

		ImGui_ImplWin32_Init(m_platformModule.GetWindow().GetHandle());
		ImGui_ImplOpenGL3_Init();
	}

	Renderer::~Renderer()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	int Renderer::Run()
	{
		bool showDemoWindow = true;
		bool showAnotherWindow = false;
		float clearColor[4]{ 0.2f, 0.3f, 0.3f, 1.0f };
		ImGuiIO& io = ImGui::GetIO();

		while (true)
		{
			if (m_platformModule.ShouldExit())
				return m_platformModule.ExitCode();

			m_inputModule.Update();
			m_platformModule.Update();

            if (m_inputModule.GetKeyUp(Input::KeyCode::Space))
                spdlog::info("space up");

			m_renderModule.Update();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			if (showDemoWindow)
				ImGui::ShowDemoWindow(&showDemoWindow);

			{
				static float f = 0.0f;
				static int counter = 0;

				ImGui::Begin("Hello World!");

				ImGui::Text("This is some useful text.");
				ImGui::Checkbox("Demo Window", &showDemoWindow);
				ImGui::Checkbox("Another Window", &showAnotherWindow);

				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
				ImGui::ColorEdit3("Color", clearColor);

				if (ImGui::Button("Button"))
					++counter;
				ImGui::SameLine();
				ImGui::Text("Counter = %d", counter);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
				ImGui::End();
			}

			if (showAnotherWindow)
			{
				ImGui::Begin("Another Window", &showAnotherWindow);
				ImGui::Text("Hello from another window!");
				if (ImGui::Button("Close Me"))
					showAnotherWindow = false;
				ImGui::End();
			}

			ImGui::Render();

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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

		return 0;
	}
}
