/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Application.h"
#include "imgui.h"
#include <ranges>

namespace TG
{
    Application::Application()
    {
        m_platformModule = std::make_unique<PlatformModule>();
        m_renderModule = std::make_unique<RenderModule>();
        m_editorModule = std::make_unique<EditorModule>();

        // 渲染模块初始化渲染器
        m_renderModule->SetupRenderer(
        	Rendering::GraphicsAPI::OpenGL,
        	m_platformModule->GetVideoPort()
        );
        m_renderModule->Subscribe(m_platformModule->onWindowResize,
			m_editorModule->onSceneViewportResize);

        // 编辑器模块接入渲染器
    	// Rendering::Renderer& renderer = m_renderModule->GetRenderer();
        m_editorModule->SetRenderer(m_renderModule->GetRenderer());

    	m_modules.emplace_back(m_renderModule.get());
    	m_modules.emplace_back(m_editorModule.get());

    	m_exampleManager = std::make_unique<ExampleManager>(m_renderModule->GetRenderer(), m_platformModule->GetTimer(), m_exampleEnum);
    	m_exampleManager->Subscribe(m_renderModule->onDraw, m_editorModule->onDrawUI,
    		m_editorModule->onSceneViewportResize);

    	m_editorModule->onDrawUI.Add( [this] { DrawUI(); });

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

            m_platformModule->PostUpdate();
        }
    }

    void Application::DrawUI()
    {
    	ImGuiIO& io = ImGui::GetIO();
    	static float lastTime = 0.0f;
    	static int frameCount = 0;
    	static float displayedFramerate = 0.0f;

    	frameCount++;
    	float currentTime = io.DeltaTime > 0 ? static_cast<float>(ImGui::GetTime()) : lastTime + io.DeltaTime;
    	if (currentTime - lastTime >= 1.0f)
    	{
    		displayedFramerate = static_cast<float>(frameCount) / (currentTime - lastTime);
    		frameCount = 0;
    		lastTime = currentTime;
    	}

        ImGui::Begin("Main Settings");
        if (ImGui::Combo("Example", &m_currentItem, m_items, IM_ARRAYSIZE(m_items)))
        {
            switch (m_currentItem)
            {
	            case 0:
	                m_exampleEnum = ExampleEnum::Triangle;
	                break;
	            case 1:
	                m_exampleEnum = ExampleEnum::Cube;
	                break;
	            case 2:
	                m_exampleEnum = ExampleEnum::BasicLight;
	                break;
				case 3:
            		m_exampleEnum = ExampleEnum::PBR;
            		break;
	            default:
	                m_exampleEnum = ExampleEnum::BasicLight;
            }
            m_exampleManager->SetCurrentExample(m_exampleEnum);
        }
    	if (m_renderModule->GetRenderer()->API() == Rendering::GraphicsAPI::OpenGL)
    		ImGui::Text("Backend: OpenGL");
    	else if (m_renderModule->GetRenderer()->API() == Rendering::GraphicsAPI::Vulkan)
    		ImGui::Text("Backend: Vulkan");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
        	1000.0f / displayedFramerate, displayedFramerate);
        ImGui::End();
    }
}
