/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "TriangleVkExample.h"
#include "Base/Utility.h"
#include "Diagnostic/Log.hpp"
#include "Exception/BaseException.h"
#include "Math/Transform/Transform.hpp"
#include "Math/Transform/AngleAxis.hpp"
#include "Rendering/ViewProjectionMatrix.hpp"
#include "stb_image.h"
#include <ranges>

namespace TG
{
    TriangleVkExample::TriangleVkExample(const Rendering::Renderer* renderer)
	    : m_renderer{ dynamic_cast<const Rendering::VulkanRenderer*>(renderer) }
    {
    	CreateVertexBuffer();
    	CreateIndexBuffer();
    	CreateUniformBuffers();
    	CreateTextureImage();
    	CreateTextureImageView();
    	CreateTextureSampler();
    	CreateDescriptorPool();
    	CreateDescriptorSetLayout();
    	CreateDescriptorSets();
        CreateGraphicsPipeline();
    }

    TriangleVkExample::~TriangleVkExample()
    {
    	VkDevice device{ m_renderer->GetContext().GetDevice() };

        vkDeviceWaitIdle(device);

        vkDestroyPipeline(device, m_graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(device, m_pipelineLayout, nullptr);
    	vkDestroyDescriptorSetLayout(device, m_descriptorSetLayout, nullptr);
    	vkDestroyDescriptorPool(device, m_descriptorPool, nullptr);
    	vkDestroySampler(device, m_textureSampler, nullptr);
    	vkDestroyImageView(device, m_textureImageView, nullptr);
    	vkFreeMemory(device, m_textureImageMemory, nullptr);
    	vkDestroyImage(device, m_textureImage, nullptr);
    	for (uint32_t i = 0; i < m_renderer->GetContext().GetMaxFramesInFlight(); ++i)
    	{
    		vkFreeMemory(device, m_uniformBuffersMemory[i], nullptr);
    		vkDestroyBuffer(device, m_uniformBuffers[i], nullptr);
    	}
    	vkFreeMemory(device, m_indexBufferMemory, nullptr);
    	vkDestroyBuffer(device, m_indexBuffer, nullptr);
    	vkFreeMemory(device, m_vertexBufferMemory, nullptr);
		vkDestroyBuffer(device, m_vertexBuffer, nullptr);
    }

	struct UniformBufferObject
    {
    	Math::Matrix4f model;
    	Math::Matrix4f view;
    	Math::Matrix4f projection;
    };

    void TriangleVkExample::Draw()
    {
    	const Rendering::VulkanContext& context{ m_renderer->GetContext() };
    	uint32_t currentFrame{ context.GetCurrentFrameIndex() };
    	VkRect2D currentRect{ m_renderer->GetSceneRect() };

    	// 更新Uniform buffer
    	static auto startTime = std::chrono::high_resolution_clock::now();
    	auto currentTime = std::chrono::high_resolution_clock::now();
    	float time = std::chrono::duration<float>(currentTime - startTime).count();
    	UniformBufferObject ubo{};
    	Math::Transform<float, 3> modelTransform;
    	modelTransform.Rotate(Math::AngleAxis{
			time * static_cast<float>(90_deg_to_rad), Math::Vector3f{ 0.0f, 0.0f, 1.0f }});
    	ubo.model = modelTransform.ToTransformMatrix();
    	ubo.view = Rendering::LookAt(Math::Vector3f{ 2.0f, 2.0f, 2.0f },
			Math::Vector3f{ 0.0f, 0.0f, 0.0f }, Math::Vector3f{ 0.0f, 0.0f, 1.0f });
    	ubo.projection = Rendering::Perspective(70.0f * Math::Deg2RadF,
			static_cast<float>(currentRect.extent.width) / static_cast<float>(currentRect.extent.height),
			0.1f, 10.0f);
    	ubo.projection[1, 1] *= -1;
    	memcpy(m_uniformBuffersMapped[currentFrame], &ubo, sizeof(ubo));

    	VkCommandBuffer cmdBuffer = context.GetCommandBuffer();

        VkRenderPassBeginInfo renderPassBeginInfo{};
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.renderPass = m_renderer->GetRenderPass();
        renderPassBeginInfo.framebuffer = m_renderer->GetFramebuffer();
        renderPassBeginInfo.renderArea = currentRect;
        VkClearValue clearValue = {{{ 0.0f, 0.0f, 0.0f, 1.0f }}};
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues = &clearValue;
        vkCmdBeginRenderPass(cmdBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);

        VkViewport viewport{};
        viewport.x = static_cast<float>(currentRect.offset.x);
        viewport.y = static_cast<float>(currentRect.offset.y);
        viewport.width = static_cast<float>(currentRect.extent.width);
        viewport.height = static_cast<float>(currentRect.extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);

        vkCmdSetScissor(cmdBuffer, 0, 1, &currentRect);

    	VkBuffer vertexBuffers[] = { m_vertexBuffer };
    	VkDeviceSize offsets[] = { 0 };
    	vkCmdBindVertexBuffers(cmdBuffer, 0, 1, vertexBuffers, offsets);

    	vkCmdBindIndexBuffer(cmdBuffer, m_indexBuffer, 0, VK_INDEX_TYPE_UINT16);

    	vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout,
    		0, 1, &m_descriptorSets[currentFrame], 0, nullptr);
        // vkCmdDraw(cmdBuffer, 3, 1, 0, 0);
    	vkCmdDrawIndexed(cmdBuffer, 6, 1, 0, 0, 0);

        vkCmdEndRenderPass(cmdBuffer);
    }

    void TriangleVkExample::DrawUI()
    {

    }

    void TriangleVkExample::OnViewportChanged(unsigned width, unsigned height)
    {

    }

    VkCommandBuffer TriangleVkExample::BeginSingleTimeCommands() const
	{
    	const Rendering::VulkanContext& context{ m_renderer->GetContext() };

    	VkCommandBufferAllocateInfo allocInfo{};
    	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    	allocInfo.commandPool = context.GetCommandPool();
    	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    	allocInfo.commandBufferCount = 1;

    	VkCommandBuffer commandBuffer;
		if (vkAllocateCommandBuffers(context.GetDevice(), &allocInfo, &commandBuffer) != VK_SUCCESS)
			throw BaseException::Create("Begin single time commands failed");

    	VkCommandBufferBeginInfo beginInfo{};
    	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    	vkBeginCommandBuffer(commandBuffer, &beginInfo);

    	return commandBuffer;
    }

	void TriangleVkExample::EndSingleTimeCommands(VkCommandBuffer commandBuffer) const
	{
    	const Rendering::VulkanContext& context{ m_renderer->GetContext() };

    	vkEndCommandBuffer(commandBuffer);

    	VkSubmitInfo submitInfo{};
    	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    	submitInfo.commandBufferCount = 1;
    	submitInfo.pCommandBuffers = &commandBuffer;

    	vkQueueSubmit(context.GetQueue(Rendering::VkQueueType::Graphic), 1, &submitInfo, VK_NULL_HANDLE);
    	vkQueueWaitIdle(context.GetQueue(Rendering::VkQueueType::Graphic));

    	vkFreeCommandBuffers(context.GetDevice(), context.GetCommandPool(), 1, &commandBuffer);
    }

	uint32_t TriangleVkExample::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const
	{
    	VkPhysicalDeviceMemoryProperties memProperties;
    	vkGetPhysicalDeviceMemoryProperties(m_renderer->GetContext().GetPhysicalDevice(), &memProperties);
    	for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
    	{
    		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
    		{
    			return i;
    		}
    	}

    	throw BaseException::Create("Failed to find suitable memory type!");
    }

	void TriangleVkExample::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
										 VkBuffer& buffer, VkDeviceMemory& bufferMemory) const
	{
    	VkDevice device = m_renderer->GetContext().GetDevice();

    	VkBufferCreateInfo bufferInfo{};
    	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    	bufferInfo.size = size;
    	bufferInfo.usage = usage;
    	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    	if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    		throw BaseException::Create("Failed to create buffer!");

    	VkMemoryRequirements memRequirements;
    	vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    	VkMemoryAllocateInfo allocInfo{};
    	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    	allocInfo.allocationSize = memRequirements.size;
    	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);
    	if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
    		throw BaseException::Create("Failed to allocate buffer memory!");

    	vkBindBufferMemory(device, buffer, bufferMemory, 0);
    }

	void TriangleVkExample::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const
	{
    	VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    	VkBufferCopy copyRegion{};
    	copyRegion.size = size;
    	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    	EndSingleTimeCommands(commandBuffer);
    }

	 void TriangleVkExample::CreateVertexBuffer()
    {
    	VkDevice device = m_renderer->GetContext().GetDevice();
    	VkDeviceSize bufferSize = sizeof(m_vertices);

    	VkBuffer stagingBuffer;
    	VkDeviceMemory stagingBufferMemory;
	    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	    	VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
	    	stagingBuffer, stagingBufferMemory);

    	void* data;
    	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    	memcpy(data, m_vertices, bufferSize);
    	vkUnmapMemory(device, stagingBufferMemory);

    	CreateBuffer(bufferSize,
    		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_vertexBuffer, m_vertexBufferMemory);

    	CopyBuffer(stagingBuffer, m_vertexBuffer, bufferSize);

    	vkDestroyBuffer(device, stagingBuffer, nullptr);
    	vkFreeMemory(device, stagingBufferMemory, nullptr);
    }

    void TriangleVkExample::CreateIndexBuffer()
    {
    	VkDevice device = m_renderer->GetContext().GetDevice();
	    VkDeviceSize bufferSize = sizeof(m_indices);

    	VkBuffer stagingBuffer;
    	VkDeviceMemory stagingBufferMemory;
    	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer, stagingBufferMemory);

    	void* data;
    	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    	memcpy(data, m_indices, bufferSize);
    	vkUnmapMemory(device, stagingBufferMemory);

    	CreateBuffer(bufferSize,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_indexBuffer, m_indexBufferMemory);

    	CopyBuffer(stagingBuffer, m_indexBuffer, bufferSize);

    	vkDestroyBuffer(device, stagingBuffer, nullptr);
    	vkFreeMemory(device, stagingBufferMemory, nullptr);
    }

    void TriangleVkExample::CreateUniformBuffers()
    {
    	VkDevice device = m_renderer->GetContext().GetDevice();
    	uint32_t maxFramesInFlight = m_renderer->GetContext().GetMaxFramesInFlight();

	    m_uniformBuffers.resize(maxFramesInFlight);
    	m_uniformBuffersMemory.resize(maxFramesInFlight);
    	m_uniformBuffersMapped.resize(maxFramesInFlight);

    	for (uint32_t i = 0; i < maxFramesInFlight; ++i)
    	{
		    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
		    CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		                 m_uniformBuffers[i],
		                 m_uniformBuffersMemory[i]);

    		vkMapMemory(device, m_uniformBuffersMemory[i], 0, bufferSize, 0, &m_uniformBuffersMapped[i]);
    	}
    }

	void TriangleVkExample::CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) const
	{
    	VkDevice device = m_renderer->GetContext().GetDevice();

    	VkImageCreateInfo imageInfo{};
    	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    	imageInfo.imageType = VK_IMAGE_TYPE_2D;
    	imageInfo.extent.width = width;
    	imageInfo.extent.height = height;
    	imageInfo.extent.depth = 1;
    	imageInfo.mipLevels = 1;
    	imageInfo.arrayLayers = 1;
    	imageInfo.format = format;
    	imageInfo.tiling = tiling;
    	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    	imageInfo.usage = usage;
    	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    	if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS)
    		throw BaseException::Create("Failed to create image!");

    	VkMemoryRequirements memRequirements;
    	vkGetImageMemoryRequirements(device, image, &memRequirements);

    	VkMemoryAllocateInfo memoryAllocateInfo{};
    	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    	memoryAllocateInfo.allocationSize = memRequirements.size;
    	memoryAllocateInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

    	if (vkAllocateMemory(device, &memoryAllocateInfo, nullptr, &imageMemory) != VK_SUCCESS)
    		throw BaseException::Create("Failed to allocate image memory!");

    	vkBindImageMemory(device, image, imageMemory, 0);
    }

	void TriangleVkExample::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) const
    {
    	VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    	VkBufferImageCopy region{};
    	region.bufferOffset = 0;
    	region.bufferRowLength = 0;
    	region.bufferImageHeight = 0;
    	region.imageOffset = { 0, 0, 0 };
    	region.imageExtent = { width, height, 1 };
    	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    	region.imageSubresource.mipLevel = 0;
    	region.imageSubresource.baseArrayLayer = 0;
    	region.imageSubresource.layerCount = 1;

    	vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    	EndSingleTimeCommands(commandBuffer);
    }

	void TriangleVkExample::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout,
		VkImageLayout newLayout) const
	{
    	VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    	VkImageMemoryBarrier barrier{};
    	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    	barrier.image = image;
    	barrier.oldLayout = oldLayout;
    	barrier.newLayout = newLayout;
    	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    	barrier.subresourceRange.baseMipLevel = 0;
    	barrier.subresourceRange.levelCount = 1;
    	barrier.subresourceRange.baseArrayLayer = 0;
    	barrier.subresourceRange.layerCount = 1;
    	barrier.srcAccessMask = 0;
    	barrier.dstAccessMask = 0;

    	VkPipelineStageFlags sourceFlags, destinationFlags;
    	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    	{
    		barrier.srcAccessMask = 0;
    		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    		sourceFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    		destinationFlags = VK_PIPELINE_STAGE_TRANSFER_BIT;
    	}
    	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    	{
    		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    		sourceFlags = VK_PIPELINE_STAGE_TRANSFER_BIT;
    		destinationFlags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    	}
    	else
    		throw BaseException::Create("Unsupported layout transition!");

    	vkCmdPipelineBarrier(commandBuffer,
			sourceFlags, destinationFlags,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

    	EndSingleTimeCommands(commandBuffer);
    }

	void TriangleVkExample::CreateTextureImage()
    {
    	VkDevice device = m_renderer->GetContext().GetDevice();

	    int width, height, channels;
    	stbi_uc* pixels = stbi_load("Assets/Textures/wall.jpg", &width, &height, &channels, STBI_rgb_alpha);
    	if (pixels == nullptr)
    		throw BaseException::Create("Failed to load image!");

    	VkDeviceSize imageSize = width * height * 4;

    	VkBuffer stagingBuffer;
    	VkDeviceMemory stagingBufferMemory;
    	CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    		stagingBuffer, stagingBufferMemory);

    	void* data;
    	vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
    	memcpy(data, pixels, imageSize);
		vkUnmapMemory(device, stagingBufferMemory);

    	stbi_image_free(pixels);

    	CreateImage(width, height, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
    		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
    		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
    		m_textureImage, m_textureImageMemory);

    	TransitionImageLayout(m_textureImage, VK_FORMAT_R8G8B8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED,
    		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    	CopyBufferToImage(stagingBuffer, m_textureImage, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
		TransitionImageLayout(m_textureImage, VK_FORMAT_R8G8B8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    	vkDestroyBuffer(device, stagingBuffer, nullptr);
    	vkFreeMemory(device, stagingBufferMemory, nullptr);
    }

    void TriangleVkExample::CreateTextureImageView()
    {
    	VkImageViewCreateInfo viewInfo{};
    	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    	viewInfo.image = m_textureImage;
    	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    	viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    	viewInfo.subresourceRange.baseMipLevel = 0;
    	viewInfo.subresourceRange.levelCount = 1;
    	viewInfo.subresourceRange.baseArrayLayer = 0;
    	viewInfo.subresourceRange.layerCount = 1;

    	if (vkCreateImageView(m_renderer->GetContext().GetDevice(), &viewInfo, nullptr, &m_textureImageView) != VK_SUCCESS)
    		throw BaseException::Create("Failed to create image view!");
    }

    void TriangleVkExample::CreateTextureSampler()
    {
	    VkSamplerCreateInfo samplerInfo{};
    	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    	samplerInfo.magFilter = VK_FILTER_LINEAR;
    	samplerInfo.minFilter = VK_FILTER_LINEAR;
    	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    	samplerInfo.anisotropyEnable = VK_TRUE;
    	VkPhysicalDeviceProperties properties{};
    	vkGetPhysicalDeviceProperties(m_renderer->GetContext().GetPhysicalDevice(), &properties);
    	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    	samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
    	samplerInfo.unnormalizedCoordinates = VK_FALSE;
    	samplerInfo.compareEnable = VK_FALSE;
    	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    	samplerInfo.mipLodBias = 0.0f;
    	samplerInfo.minLod = 0.0f;
    	samplerInfo.maxLod = 0.0f;

    	if (vkCreateSampler(m_renderer->GetContext().GetDevice(), &samplerInfo, nullptr, &m_textureSampler) != VK_SUCCESS)
    		throw BaseException::Create("Failed to create texture sampler!");
    }

	VkShaderModule TriangleVkExample::CreateShaderModule(const std::vector<char>& code) const
	{
    	VkShaderModuleCreateInfo createInfo{};
    	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    	createInfo.codeSize = code.size();
    	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
    	VkShaderModule shaderModule;
    	if (vkCreateShaderModule(m_renderer->GetContext().GetDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
    		throw BaseException::Create("Failed to create shader module!");
    	return shaderModule;
    }

	void TriangleVkExample::CreateDescriptorPool()
	{
    	uint32_t maxFrameInFlight = m_renderer->GetContext().GetMaxFramesInFlight();

    	std::array<VkDescriptorPoolSize, 2> poolSizes{};
    	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    	poolSizes[0].descriptorCount = maxFrameInFlight;
    	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    	poolSizes[1].descriptorCount = maxFrameInFlight;

    	VkDescriptorPoolCreateInfo poolInfo{};
    	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    	poolInfo.pPoolSizes = poolSizes.data();
    	poolInfo.maxSets = maxFrameInFlight;

    	if (vkCreateDescriptorPool(m_renderer->GetContext().GetDevice(), &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS)
    		throw BaseException::Create("Failed to create descriptor pool!");
	}

	void TriangleVkExample::CreateDescriptorSetLayout()
    {
    	VkDescriptorSetLayoutBinding uboLayoutBinding{};
    	uboLayoutBinding.binding = 0;
    	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    	uboLayoutBinding.descriptorCount = 1;
    	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    	samplerLayoutBinding.binding = 1;
    	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    	samplerLayoutBinding.descriptorCount = 1;
    	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    	std::array<VkDescriptorSetLayoutBinding, 2> bindings{ uboLayoutBinding, samplerLayoutBinding };
		VkDescriptorSetLayoutCreateInfo layoutCreateInfo{};
    	layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    	layoutCreateInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutCreateInfo.pBindings = bindings.data();

    	if (vkCreateDescriptorSetLayout(m_renderer->GetContext().GetDevice(), &layoutCreateInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS)
    		throw BaseException::Create("Failed to create descriptor set layout!");
    }

    void TriangleVkExample::CreateDescriptorSets()
    {
    	VkDevice device = m_renderer->GetContext().GetDevice();
    	uint32_t maxFramesInFlight = m_renderer->GetContext().GetMaxFramesInFlight();

		std::vector<VkDescriptorSetLayout> descriptorSetLayouts(maxFramesInFlight, m_descriptorSetLayout);
    	VkDescriptorSetAllocateInfo allocInfo{};
    	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    	allocInfo.descriptorPool = m_descriptorPool;
    	allocInfo.descriptorSetCount = maxFramesInFlight;
    	allocInfo.pSetLayouts = descriptorSetLayouts.data();

    	m_descriptorSets.resize(maxFramesInFlight);
    	if (vkAllocateDescriptorSets(device, &allocInfo, m_descriptorSets.data()) != VK_SUCCESS)
    		throw BaseException::Create("Failed to allocate descriptor sets!");

    	for (uint32_t i = 0; i < maxFramesInFlight; ++i)
    	{
    		VkDescriptorBufferInfo bufferInfo{};
    		bufferInfo.buffer = m_uniformBuffers[i];
    		bufferInfo.offset = 0;
    		bufferInfo.range = VK_WHOLE_SIZE;

    		VkDescriptorImageInfo imageInfo{};
    		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    		imageInfo.imageView = m_textureImageView;
    		imageInfo.sampler = m_textureSampler;

    		std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
			descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    		descriptorWrites[0].dstSet = m_descriptorSets[i];
    		descriptorWrites[0].dstBinding = 0;
    		descriptorWrites[0].dstArrayElement = 0;
    		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    		descriptorWrites[0].descriptorCount = 1;
    		descriptorWrites[0].pBufferInfo = &bufferInfo;
    		descriptorWrites[0].pImageInfo = nullptr;
    		descriptorWrites[0].pTexelBufferView = nullptr;

    		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    		descriptorWrites[1].dstSet = m_descriptorSets[i];
    		descriptorWrites[1].dstBinding = 1;
    		descriptorWrites[1].dstArrayElement = 0;
    		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    		descriptorWrites[1].descriptorCount = 1;
    		descriptorWrites[1].pBufferInfo = nullptr;
    		descriptorWrites[1].pImageInfo = &imageInfo;
    		descriptorWrites[1].pTexelBufferView = nullptr;

    		vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()),
    			descriptorWrites.data(), 0, nullptr);
    	}
    }

    void TriangleVkExample::CreateGraphicsPipeline()
    {
        // 创建顶点着色器
        std::vector<char> vertShaderCode = LoadBinaryFile("Assets/Shaders/GLSL/Vulkan/simple_vert.spv");
        VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);
        VkPipelineShaderStageCreateInfo vertShaderStageCreateInfo = {};
        vertShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageCreateInfo.module = vertShaderModule;
        vertShaderStageCreateInfo.pName = "main";
        // 创建片元着色器
        std::vector<char> fragShaderCode = LoadBinaryFile("Assets/Shaders/GLSL/Vulkan/simple_frag.spv");
        VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);
        VkPipelineShaderStageCreateInfo fragShaderStageCreateInfo = {};
        fragShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageCreateInfo.module = fragShaderModule;
        fragShaderStageCreateInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageCreateInfo, fragShaderStageCreateInfo };

    	VkVertexInputBindingDescription vertexInputBindingDescription{};
    	vertexInputBindingDescription.binding = 0;
    	vertexInputBindingDescription.stride =  7 * sizeof(float);
    	vertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    	std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
    	attributeDescriptions[0].binding = 0;
    	attributeDescriptions[0].location = 0;
    	attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    	attributeDescriptions[0].offset = 0;

    	attributeDescriptions[1].binding = 0;
    	attributeDescriptions[1].location = 1;
    	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    	attributeDescriptions[1].offset = 2 * sizeof(float);

    	attributeDescriptions[2].binding = 0;
    	attributeDescriptions[2].location = 2;
    	attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    	attributeDescriptions[2].offset = 5 * sizeof(float);
        // 顶点输入状态
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &vertexInputBindingDescription;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
        // 顶点装配器
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
        inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
        // 视口和裁剪
        VkPipelineViewportStateCreateInfo viewportStateCreateInfo{};
        viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportStateCreateInfo.viewportCount = 1;
        viewportStateCreateInfo.scissorCount = 1;
        // 需要动态改变的状态
        std::vector dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR,
        };
        VkPipelineDynamicStateCreateInfo dynamicStatesCreateInfo{};
        dynamicStatesCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicStatesCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicStatesCreateInfo.pDynamicStates = dynamicStates.data();
        // 光栅化状态
        VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo{};
        rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
        rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
        rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizationStateCreateInfo.lineWidth = 1.0f;
        rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
        rasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
        rasterizationStateCreateInfo.depthBiasClamp = 0.0f;
        rasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;
        // 不开启MSAA
        VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo{};
        multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
        multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampleStateCreateInfo.minSampleShading = 1.0f;
        multisampleStateCreateInfo.pSampleMask = nullptr;
        multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
        multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;
        // 颜色混合
        VkPipelineColorBlendAttachmentState colorBlendAttachmentState{};
        colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachmentState.blendEnable = VK_FALSE;
        colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
        VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo{};
        colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
        colorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
        colorBlendStateCreateInfo.attachmentCount = 1;
        colorBlendStateCreateInfo.pAttachments = &colorBlendAttachmentState;
        colorBlendStateCreateInfo.blendConstants[0] = 0.0f;
        colorBlendStateCreateInfo.blendConstants[1] = 0.0f;
        colorBlendStateCreateInfo.blendConstants[2] = 0.0f;
        colorBlendStateCreateInfo.blendConstants[3] = 0.0f;
        // Pipeline layout
        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
        pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutCreateInfo.setLayoutCount = 1;
        pipelineLayoutCreateInfo.pSetLayouts = &m_descriptorSetLayout;
        pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
        pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;
    	VkDevice device = m_renderer->GetContext().GetDevice();
        if (vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
            throw BaseException::Create("Failed to create pipeline layout!");

        VkGraphicsPipelineCreateInfo pipelineCreateInfo{};
        pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineCreateInfo.stageCount = 2;
        pipelineCreateInfo.pStages = shaderStages;
        pipelineCreateInfo.pVertexInputState = &vertexInputInfo;
        pipelineCreateInfo.pInputAssemblyState = &inputAssemblyInfo;
        pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
        pipelineCreateInfo.pDynamicState = &dynamicStatesCreateInfo;
        pipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;
        pipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;
        pipelineCreateInfo.pDepthStencilState = nullptr;
        pipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo;
        pipelineCreateInfo.layout = m_pipelineLayout;
        pipelineCreateInfo.renderPass = m_renderer->GetRenderPass();
        pipelineCreateInfo.subpass = 0;
        pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineCreateInfo.basePipelineIndex = -1;
        if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr,
            &m_graphicsPipeline) != VK_SUCCESS)
        {
            throw BaseException::Create("Failed to create graphics pipeline!");
        }

        vkDestroyShaderModule(device, vertShaderModule, nullptr);
        vkDestroyShaderModule(device, fragShaderModule, nullptr);
    }
}
