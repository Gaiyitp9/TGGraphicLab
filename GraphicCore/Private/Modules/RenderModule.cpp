/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Modules/RenderModule.h"
#include "Rendering/OpenGL/OpenGLRenderer.h"
#include "Rendering/VulkanExample/VulkanRenderer.h"

namespace TG
{
    RenderModule::RenderModule() = default;

    RenderModule::~RenderModule() = default;

    void RenderModule::Update()
    {
    	if (m_Renderer)
			m_Renderer->Render();
    }

	void RenderModule::PostUpdate()
	{
    	if (m_Renderer)
    		m_Renderer->Present();
	}

    void RenderModule::SetupRenderer(const IDefaultVideoPort& videoPort, const ITimer& timer)
    {
    	// 不能同时初始化Vulkan和OpenGLES，否则会报错，原因暂时未知(可能是不能使用相同窗口来初始化)
        switch (m_GraphicsAPI)
        {
            case Rendering::GraphicsAPI::OpenGL:
    	        m_Renderer = std::make_unique<Rendering::OpenGLRenderer>(videoPort, timer);
                break;
            case Rendering::GraphicsAPI::Vulkan:
                m_Renderer = std::make_unique<Rendering::VulkanRenderer>(videoPort);
                break;
            case Rendering::GraphicsAPI::DirectX11:
                // m_Renderer = std::make_unique<Rendering::OpenGLRenderer>(videoPort, timer);
                break;
            case Rendering::GraphicsAPI::DirectX12:
                // m_Renderer = std::make_unique<Rendering::OpenGLRenderer>(videoPort, timer);
                break;
        }
    }

	void RenderModule::Subscribe(MulticastDelegate<void(unsigned, unsigned)>& windowResizeDelegate)
	{
		windowResizeDelegate.Add(
		    [&renderer = std::as_const(m_Renderer)](unsigned width, unsigned height) {
			    renderer->FrameBufferResizeCallback(width, height);
		    }
		);
	}
}
