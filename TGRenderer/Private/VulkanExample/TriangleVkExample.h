/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Example.h"
#include "Rendering/Vulkan/VulkanRenderer.h"

namespace TG
{
    class TriangleVkExample : public Example
    {
    public:
        explicit TriangleVkExample(const Rendering::Renderer* renderer);
        ~TriangleVkExample() override;

        void Draw() override;
    	void DrawUI() override;

    	void OnViewportChanged(unsigned width, unsigned height) override;

    private:
    	VkCommandBuffer BeginSingleTimeCommands() const;
    	void EndSingleTimeCommands(VkCommandBuffer commandBuffer) const;

    	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
    	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
    		VkBuffer& buffer, VkDeviceMemory& bufferMemory) const;
    	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const;
    	void CreateVertexBuffer();
    	void CreateIndexBuffer();
    	void CreateUniformBuffers();

    	void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
    		VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) const;
    	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) const;
    	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) const;
    	void CreateTextureImage();
    	void CreateTextureImageView();
    	void CreateTextureSampler();

        VkShaderModule CreateShaderModule(const std::vector<char>& code) const;
    	void CreateDescriptorPool();
    	void CreateDescriptorSetLayout();
    	void CreateDescriptorSets();
        void CreateGraphicsPipeline();

    	const Rendering::VulkanRenderer* m_renderer;

    	VkBuffer m_vertexBuffer{ VK_NULL_HANDLE };
    	VkDeviceMemory m_vertexBufferMemory{ VK_NULL_HANDLE };
    	float m_vertices[28] =
    	{
    		-0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    		-0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
    	};

    	VkBuffer m_indexBuffer{ VK_NULL_HANDLE };
    	VkDeviceMemory m_indexBufferMemory{ VK_NULL_HANDLE };
    	uint16_t m_indices[6] = { 0, 1, 2, 2, 3, 0 };

    	std::vector<VkBuffer> m_uniformBuffers;
    	std::vector<VkDeviceMemory> m_uniformBuffersMemory;
    	std::vector<void*> m_uniformBuffersMapped;

    	VkImage m_textureImage{ VK_NULL_HANDLE };
    	VkDeviceMemory m_textureImageMemory{ VK_NULL_HANDLE };
    	VkImageView m_textureImageView{ VK_NULL_HANDLE };
    	VkSampler m_textureSampler{ VK_NULL_HANDLE };

    	VkDescriptorPool m_descriptorPool{ VK_NULL_HANDLE };
    	VkDescriptorSetLayout m_descriptorSetLayout{ VK_NULL_HANDLE };
    	std::vector<VkDescriptorSet> m_descriptorSets;

        VkPipelineLayout m_pipelineLayout{ VK_NULL_HANDLE };
        VkPipeline m_graphicsPipeline{ VK_NULL_HANDLE };
    };
}
