/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Application.h"
#include "OpenGLExample/OpenGLExampleFactory.h"
// #include "VulkanExample/VulkanExampleFactory.h"
#include "imgui.h"
#include <ranges>

namespace TG
{
    Application::Application()
    {
        m_platformModule = std::make_unique<PlatformModule>();
        m_renderModule = std::make_unique<RenderModule>();
        m_editorModule = std::make_unique<EditorModule>();

        // 渲染模块接入视频接口
        m_renderModule->SetupRenderer(m_platformModule->GetVideoPort());
        m_renderModule->Subscribe(m_platformModule->onWindowResize,
            m_editorModule->onSceneViewportResize);
        // 编辑器模块接入渲染器
        m_editorModule->SetRenderer(m_renderModule->GetRenderer());

        m_modules.emplace_back(m_renderModule.get());
        m_modules.emplace_back(m_editorModule.get());

        // std::string_view mbstr = "z\u00df\u6c34\U0001f34c";
        // std::wstring_view wcstr = L"z\u00df\u6c34\U0001f34c";

        m_exampleFactory = std::make_unique<OpenGLExampleFactory>(
            m_platformModule->GetVideoPort(),
            m_platformModule->GetTimer());
        m_example = m_exampleFactory->CreateExample(m_exampleEnum);

        m_renderModule->onDraw.Add(
            [&example = m_example]{
                example->Draw();
            }
        );
        m_editorModule->onDrawUI.Add(
            [&example = m_example]{
                example->DrawUI();
            }
        );
        m_editorModule->onDrawUI.Add(
            [this]{
                this->DrawUI();
            }
        );
    	// m_triangleVkExample = std::make_unique<TriangleVkExample>(m_platformModule->GetVideoPort());
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

        	// m_triangleVkExample->Draw();

            for (const auto& module : m_modules | std::views::reverse)
                module->PostUpdate();

            m_platformModule->PostUpdate();
        }
    }

    void Application::DrawUI()
    {
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
	            default:
	                m_exampleEnum = ExampleEnum::BasicLight;
            }
            m_example = m_exampleFactory->CreateExample(m_exampleEnum);
        }
        ImGui::End();
    }
}
