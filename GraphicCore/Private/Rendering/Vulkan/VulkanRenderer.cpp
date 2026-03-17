/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Rendering/Vulkan/VulkanRenderer.h"

namespace TG::Rendering
{
	VulkanRenderer::VulkanRenderer(const IDefaultVideoPort& videoPort)
		: m_context(videoPort)
	{
		CreateRenderPass();
		CreateFramebuffers();

		CreateOffscreenRenderPass();
		CreateOffscreenFramebuffer();
	}

	VulkanRenderer::~VulkanRenderer()
	{
		VkDevice device = m_context.GetDevice();

		CleanupOffscreenFramebuffer();
		vkDestroyRenderPass(device, m_offscreenRenderPass, nullptr);

		CleanupFramebuffers();
		vkDestroyRenderPass(device, m_renderPass, nullptr);
	}

	GraphicsAPI VulkanRenderer::API()
	{
		return GraphicsAPI::Vulkan;
	}

	const VulkanContext& VulkanRenderer::GetContext() const
	{
		return m_context;
	}

	void VulkanRenderer::BeginRender()
	{
		if (!m_context.AcquireNextImage())
			RecreateFramebuffers();

		m_context.BeginCommandBuffer();
	}

	void VulkanRenderer::EndRender()
	{

	}

	void VulkanRenderer::Draw(Mesh const* mesh, Material const* material)
	{

	}

	void VulkanRenderer::Present()
	{
		m_context.EndCommandBuffer();
		m_context.Submit();
		if (!m_context.Present())
			RecreateFramebuffers();
	}

	void VulkanRenderer::ScreenFramebufferResizeCallback(unsigned int width, unsigned int height)
	{
		m_screenWidth = width;
		m_screenHeight = height;
	}

	void VulkanRenderer::SceneViewportChangedCallback(int posX, int posY, unsigned int width, unsigned int height)
	{
		m_scenePosX = posX;
		m_scenePosY = posY;
		m_sceneWidth = width;
		m_sceneHeight = height;
	}

	void VulkanRenderer::RenderToScene()
	{

	}

	void VulkanRenderer::RenderToUI()
	{

	}

	void VulkanRenderer::RenderToTexture()
	{

	}

	Texture const* VulkanRenderer::RenderTarget()
	{
		return nullptr;
	}

	void VulkanRenderer::SetVSync(bool enable) const
	{

	}

	VkRect2D VulkanRenderer::GetSceneRect() const
	{
		return { m_scenePosX, m_scenePosY, m_sceneWidth, m_sceneHeight };
	}

	VkRenderPass VulkanRenderer::GetRenderPass() const
	{
		return m_renderPass;
	}

	VkFramebuffer VulkanRenderer::GetFramebuffer() const
	{
		return m_swapChainFrameBuffers[m_context.GetCurrentImageIndex()];
	}

	void VulkanRenderer::CreateRenderPass()
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = m_context.GetSwapChainFormat();
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(m_context.GetDevice(), &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS)
			throw BaseException::Create("Failed to create render pass!");
	}

	void VulkanRenderer::CreateFramebuffers()
	{
		VkDevice device = m_context.GetDevice();
		const std::vector<VkImageView>& swapChainImageViews = m_context.GetSwapChainImageViews();
		VkExtent2D swapChainExtent = m_context.GetSwapChainExtent();
		uint32_t imageCount = swapChainImageViews.size();

		m_swapChainFrameBuffers.resize(imageCount);
		for (std::size_t i = 0; i < imageCount; ++i)
		{
			VkImageView attachments[] = { swapChainImageViews[i] };

			VkFramebufferCreateInfo framebufferCreateInfo{};
			framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferCreateInfo.renderPass = m_renderPass;
			framebufferCreateInfo.attachmentCount = 1;
			framebufferCreateInfo.pAttachments = attachments;
			framebufferCreateInfo.width = swapChainExtent.width;
			framebufferCreateInfo.height = swapChainExtent.height;
			framebufferCreateInfo.layers = 1;
			if (vkCreateFramebuffer(device, &framebufferCreateInfo, nullptr,
				&m_swapChainFrameBuffers[i]) != VK_SUCCESS)
			{
				throw BaseException::Create("Failed to create framebuffer!");
			}
		}
	}

	void VulkanRenderer::CleanupFramebuffers()
	{
		VkDevice device = m_context.GetDevice();

		for (int i = 0; i < m_swapChainFrameBuffers.size(); ++i)
			vkDestroyFramebuffer(device, m_swapChainFrameBuffers[i], nullptr);
		m_swapChainFrameBuffers.clear();
	}

	void VulkanRenderer::RecreateFramebuffers()
	{
		CleanupFramebuffers();
		CreateFramebuffers();
	}

	void VulkanRenderer::CreateOffscreenRenderPass()
	{
		VkAttachmentDescription attachmentDescription{};
		attachmentDescription.format = m_offscreenFormat;
		attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
		attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		VkAttachmentReference attachmentReference{};
		attachmentReference.attachment = 0;
		attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &attachmentReference;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		dependency.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &attachmentDescription;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;
		if (vkCreateRenderPass(m_context.GetDevice(), &renderPassInfo, nullptr, &m_offscreenRenderPass) != VK_SUCCESS)
			throw BaseException::Create("Failed to create render pass!");
	}

	uint32_t VulkanRenderer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(m_context.GetPhysicalDevice(), &memProperties);
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
		{
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		throw BaseException::Create("Failed to find suitable memory type!");
	}

	void VulkanRenderer::CreateOffscreenFramebuffer()
	{
		if (m_sceneWidth == 0 || m_sceneHeight == 0)
			return;

		m_offscreenTexture.resize(m_context.GetMaxFramesInFlight());
		m_offscreenTextureImageMemory.resize(m_context.GetMaxFramesInFlight());
		m_offscreenTextureImageView.resize(m_context.GetMaxFramesInFlight());
		m_offscreenFramebuffers.resize(m_context.GetMaxFramesInFlight());

		VkDevice device = m_context.GetDevice();
		for (uint32_t i = 0; i < m_context.GetMaxFramesInFlight(); ++i)
		{
			VkImageCreateInfo imageInfo{};
			imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			imageInfo.imageType = VK_IMAGE_TYPE_2D;
			imageInfo.format = m_offscreenFormat;
			imageInfo.extent.width = m_sceneWidth;
			imageInfo.extent.height = m_sceneHeight;
			imageInfo.extent.depth = 1;
			imageInfo.mipLevels = 1;
			imageInfo.arrayLayers = 1;
			imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
			imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			if (vkCreateImage(device, &imageInfo, nullptr, &m_offscreenTexture[i]) != VK_SUCCESS)
				throw BaseException::Create("Failed to create offscreen framebuffer texture");

			VkMemoryRequirements memRequirements;
			vkGetImageMemoryRequirements(device, m_offscreenTexture[i], &memRequirements);

			VkMemoryAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			if (vkAllocateMemory(device, &allocInfo, nullptr, &m_offscreenTextureImageMemory[i]) != VK_SUCCESS)
				throw BaseException::Create("Failed to allocate offscreen framebuffer image memory");

			vkBindImageMemory(device, m_offscreenTexture[i], m_offscreenTextureImageMemory[i], 0);

			VkImageViewCreateInfo viewInfo{};
			viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewInfo.image = m_offscreenTexture[i];
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			viewInfo.format = imageInfo.format;
			viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			viewInfo.subresourceRange.baseMipLevel = 0;
			viewInfo.subresourceRange.levelCount = 1;
			viewInfo.subresourceRange.baseArrayLayer = 0;
			viewInfo.subresourceRange.layerCount = 1;
			if (vkCreateImageView(device, &viewInfo, nullptr, &m_offscreenTextureImageView[i]) != VK_SUCCESS)
				throw BaseException::Create("Failed to create offscreen framebuffer image view");

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = m_offscreenRenderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = &m_offscreenTextureImageView[i];
			framebufferInfo.width = m_sceneWidth;
			framebufferInfo.height = m_sceneHeight;
			framebufferInfo.layers = 1;
			if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &m_offscreenFramebuffers[i]) != VK_SUCCESS)
				throw BaseException::Create("Failed to create framebuffer!");
		}

		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;
		if (vkCreateSampler(device, &samplerInfo, nullptr, &m_offscreenTextureSampler) != VK_SUCCESS)
			throw BaseException::Create("Failed to create offscreen framebuffer sampler");
	}

	void VulkanRenderer::CleanupOffscreenFramebuffer()
	{
		VkDevice device = m_context.GetDevice();

		vkDestroySampler(device, m_offscreenTextureSampler, nullptr);

		for (uint32_t i = 0; i < m_offscreenTextureImageView.size(); i++)
		{
			vkDestroyImageView(device, m_offscreenTextureImageView[i], nullptr);
			vkFreeMemory(device, m_offscreenTextureImageMemory[i], nullptr);
			vkDestroyImage(device, m_offscreenTexture[i], nullptr);
			vkDestroyFramebuffer(device, m_offscreenFramebuffers[i], nullptr);
		}
		m_offscreenFramebuffers.clear();
		m_offscreenTextureImageView.clear();
		m_offscreenTextureImageMemory.clear();
		m_offscreenFramebuffers.clear();
	}

	void VulkanRenderer::RecreateOffscreenFramebuffer()
	{
		CleanupOffscreenFramebuffer();
		CreateOffscreenFramebuffer();
	}
}
