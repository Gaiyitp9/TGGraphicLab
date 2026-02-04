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
        m_renderModule = std::make_unique<RenderModule>();
        m_editorModule = std::make_unique<EditorModule>();

        // 初始化渲染模块，接入视频接口和计时器接口
        m_renderModule->SetupRenderer(m_platformModule->GetVideoPort(), m_platformModule->GetTimer());
        m_renderModule->Subscribe(m_platformModule->onWindowResize);
        // 编辑器模块设置渲染器，并接入视频接口
        m_editorModule->SetRenderer(m_renderModule->GetRenderer());
        m_editorModule->PlugInVideoPort(m_platformModule->GetVideoPort());

        m_modules.emplace_back(m_renderModule.get());
        m_modules.emplace_back(m_editorModule.get());

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
}
