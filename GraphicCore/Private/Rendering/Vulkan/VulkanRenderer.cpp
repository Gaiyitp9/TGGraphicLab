/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Rendering/Vulkan/VulkanRenderer.h"

namespace TG::Rendering
{
	VulkanRenderer::VulkanRenderer(const IDefaultVideoPort& videoPort)
		: m_videoPort(videoPort)/*, m_context(videoPort)*/
	{

	}

	VulkanRenderer::~VulkanRenderer() = default;

	char const* VulkanRenderer::Type()
	{
		return "Vulkan";
	}

	const IDefaultVideoPort& VulkanRenderer::VideoPort() const
	{
		return m_videoPort;
	}

	void VulkanRenderer::PreRender()
	{

	}

	void VulkanRenderer::Draw(Mesh const* mesh, Material const* material)
	{

	}

	void VulkanRenderer::Present()
	{

	}

	void VulkanRenderer::ScreenFrameBufferResizeCallback(unsigned int width, unsigned int height)
	{

	}

	void VulkanRenderer::SceneFrameBufferResizeCallback(unsigned int width, unsigned int height)
	{

	}

	void VulkanRenderer::RenderToScreen() const
	{

	}

	void VulkanRenderer::RenderToTexture() const
	{

	}

	Texture const* VulkanRenderer::RenderTarget()
	{
		return nullptr;
	}

	void VulkanRenderer::SetVSync(bool enable) const
	{

	}
}
