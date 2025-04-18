/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Modules/RenderModule.h"
#include "Rendering/OpenGLESExample/OpenGLESRenderer.h"
// #include "Rendering/VulkanSample/VulkanRenderer.h"

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

    void RenderModule::PlugInVideoPort(const IDefaultVideoPort& videoPort, const ITimer& timer)
    {
    	// 不能同时初始化Vulkan和OpenGLES，否则会报错，原因暂时未知(可能是不能使用相同窗口来初始化)
    	// m_Renderer = std::make_shared<VulkanRenderer>(videoPort.GetHandle());
    	m_Renderer = std::make_shared<OpenGLESRenderer>(videoPort.GetHandle(), videoPort.GetContext(), timer);
    }

	void RenderModule::Subscribe(MulticastDelegate<void(unsigned int, unsigned int)>& windowResizeDelegate)
	{
		windowResizeDelegate.Add([&renderer = m_Renderer](unsigned int width, unsigned int height) {
			renderer->FrameBufferResizeCallback(width, height);
		});
	}
}
