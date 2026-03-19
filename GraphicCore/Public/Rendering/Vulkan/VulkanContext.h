/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/Context.h"
#include "vulkan/vulkan.h"

namespace TG::Rendering
{
	enum class VkQueueType
	{
		Graphic = VK_QUEUE_GRAPHICS_BIT,
		Compute = VK_QUEUE_COMPUTE_BIT,
		Transfer = VK_QUEUE_TRANSFER_BIT,
		Present,
	};

	class VulkanContext : public Context
	{
	public:
		explicit VulkanContext(const IDefaultVideoPort& videoPort);
		~VulkanContext() override;

		VulkanContext(const VulkanContext&) = delete;
		VulkanContext(VulkanContext&&) = delete;
		VulkanContext& operator=(const VulkanContext&) = delete;
		VulkanContext& operator=(VulkanContext&&) = delete;

		[[nodiscard]] const IDefaultVideoPort& VideoPort() const override;

		[[nodiscard]] VkInstance GetInstance() const;
		[[nodiscard]] VkDevice GetDevice() const;
		[[nodiscard]] VkPhysicalDevice GetPhysicalDevice() const;
		[[nodiscard]] uint32_t GetQueueFamily(VkQueueType type) const;
		[[nodiscard]] VkQueue GetQueue(VkQueueType type) const;
		[[nodiscard]] VkFormat GetSwapChainFormat() const;
		[[nodiscard]] uint32_t GetSwapChainMinImageCount() const;
		[[nodiscard]] uint32_t GetSwapChainImageCount() const;
		[[nodiscard]] const std::vector<VkImage>& GetSwapChainImages() const;
		[[nodiscard]] const std::vector<VkImageView>& GetSwapChainImageViews() const;
		[[nodiscard]] VkExtent2D GetSwapChainExtent() const;
		[[nodiscard]] uint32_t GetCurrentImageIndex() const;
		[[nodiscard]] VkCommandPool GetCommandPool() const;
		[[nodiscard]] VkCommandBuffer GetCommandBuffer() const;
		[[nodiscard]] uint32_t GetMaxFramesInFlight() const;
		[[nodiscard]] uint32_t GetCurrentFrameIndex() const;

		bool AcquireNextImage();
		void BeginCommandBuffer() const;
		void EndCommandBuffer() const;
		void Submit();
		bool Present();

	private:
		void CheckLayerAndExtension();
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData
		);
		void CreateInstance();
		void SetupDebugMessenger();
		void CreateSurface(HWND handle);
        void SelectPhysicalDevice();
		bool IsDeviceSuitable(VkPhysicalDevice device);
		void CreateLogicalDevice();
		void CreateSwapChain();
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat() const;
		VkPresentModeKHR ChooseSwapPresentMode() const;
        void CleanupSwapChain();
		void RecreateSwapChain();
		void CreateCommandPool();
        void CreateCommandBuffers();
        void CreateSyncObjects();

		const IDefaultVideoPort& m_videoPort;

		bool m_enableValidationLayer { true };
		std::vector<char const*> m_requiredVulkanLayers;
		std::vector<char const*> m_requiredVulkanExtensions{
			VK_KHR_SURFACE_EXTENSION_NAME,
			"VK_KHR_win32_surface",
		};

		VkInstance m_instance{ VK_NULL_HANDLE };
		VkDebugUtilsMessengerEXT m_debugMessenger{ VK_NULL_HANDLE };
		VkSurfaceKHR m_surface{ VK_NULL_HANDLE };

		std::vector<char const*> m_requiredDeviceExtensions{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
		};
		std::vector<VkQueueType> m_requiredQueueFamilies{
			VkQueueType::Graphic,
			VkQueueType::Compute,
			VkQueueType::Transfer,
			VkQueueType::Present,
		};
		VkPhysicalDeviceFeatures m_deviceFeatures{
			.samplerAnisotropy = VK_TRUE
		};
		VkPhysicalDevice m_physicalDevice{ VK_NULL_HANDLE };
		std::unordered_map<VkQueueType, uint32_t> m_familyIndices;

		VkDevice m_device{ VK_NULL_HANDLE };
		std::unordered_map<VkQueueType, VkQueue> m_queues;

		VkSurfaceCapabilitiesKHR m_capabilities{};
		std::vector<VkSurfaceFormatKHR> m_swapChainFormats;
		std::vector<VkPresentModeKHR> m_presentModes;
		VkSwapchainKHR m_swapChain{ VK_NULL_HANDLE };
		VkSwapchainKHR m_oldSwapChain{ VK_NULL_HANDLE };

		std::vector<VkImage> m_swapChainImages;
		VkFormat m_swapChainImageFormat{ VK_FORMAT_UNDEFINED };
		VkExtent2D m_swapChainExtent{};
		std::vector<VkImageView> m_swapChainImageViews;

		const uint32_t MAX_FRAMES_IN_FLIGHT = 3;
		VkCommandPool m_cmdPool{ VK_NULL_HANDLE };
		std::vector<VkCommandBuffer> m_cmdBuffers;

		std::vector<VkSemaphore> m_imageAvailableSemaphores;
		std::vector<VkSemaphore> m_renderFinishedSemaphores;
		std::vector<VkFence> m_inFlightFences;
		uint32_t m_currentFrame { 0 };
		uint32_t m_imageIndex{ 0 };
	};
}
