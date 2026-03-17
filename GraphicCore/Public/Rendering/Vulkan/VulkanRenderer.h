/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/Renderer.h"
#include "Rendering/Vulkan/VulkanContext.h"

namespace TG::Rendering
{
	class VulkanRenderer : public Renderer
	{
	public:
		explicit VulkanRenderer(const IDefaultVideoPort& videoPort);
		~VulkanRenderer() override;

		VulkanRenderer(const VulkanRenderer&) = delete;
		VulkanRenderer(VulkanRenderer&&) = delete;
		VulkanRenderer& operator=(const VulkanRenderer&) = delete;
		VulkanRenderer& operator=(VulkanRenderer&&) = delete;

		GraphicsAPI API() override;
		
		[[nodiscard]] const VulkanContext& GetContext() const override;

		void BeginRender() override;
		void EndRender() override;

		void Draw(Mesh const* mesh, Material const* material) override;
		void Present() override;

		void ScreenFramebufferResizeCallback(unsigned int width, unsigned int height) override;
		void SceneViewportChangedCallback(int posX, int posY, unsigned int width, unsigned int height) override;

		void RenderToScene() override;
		void RenderToUI() override;

		void RenderToTexture() override;
		Texture const* RenderTarget() override;

    	void SetVSync(bool enable) const override;

		[[nodiscard]] VkRect2D GetSceneRect() const;
		[[nodiscard]] VkRenderPass GetRenderPass() const;
		[[nodiscard]] VkFramebuffer GetFramebuffer() const;

	private:
		void CreateRenderPass();
		void CreateFramebuffers();
		void CleanupFramebuffers();
		void RecreateFramebuffers();

		void CreateOffscreenRenderPass();
		[[nodiscard]] uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
		void CreateOffscreenFramebuffer();
		void CleanupOffscreenFramebuffer();
		void RecreateOffscreenFramebuffer();

		VulkanContext m_context;

		unsigned int m_screenWidth{ 1 };
		unsigned int m_screenHeight{ 1 };

		VkRenderPass m_renderPass{ VK_NULL_HANDLE };
		// VkImage m_depthImage{ VK_NULL_HANDLE };
		// VkDeviceMemory m_depthImageMemory{ VK_NULL_HANDLE };
		// VkImageView m_depthImageView{ VK_NULL_HANDLE };
		std::vector<VkFramebuffer> m_swapChainFrameBuffers;

		int m_scenePosX{ 0 };
		int m_scenePosY{ 0 };
		unsigned int m_sceneWidth{ 1 };
		unsigned int m_sceneHeight{ 1 };

		VkRenderPass m_offscreenRenderPass{ VK_NULL_HANDLE };
		int m_framebufferIndex{ 0 };
		VkFormat m_offscreenFormat{ VK_FORMAT_R8G8B8A8_SRGB };
		std::vector<VkImage> m_offscreenTexture;
		std::vector<VkDeviceMemory> m_offscreenTextureImageMemory;
		std::vector<VkImageView> m_offscreenTextureImageView;
		VkSampler m_offscreenTextureSampler{ VK_NULL_HANDLE };
		std::vector<VkFramebuffer> m_offscreenFramebuffers;
	};
}
