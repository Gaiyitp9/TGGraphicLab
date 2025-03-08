/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Renderer.h"
#include "Base/WindowBase.hpp"
#include "vulkan/vulkan.h"

namespace TG
{
    enum class VkQueueType
    {
        Graphic = VK_QUEUE_GRAPHICS_BIT,
        Compute = VK_QUEUE_COMPUTE_BIT,
        Transfer = VK_QUEUE_TRANSFER_BIT,
        Present,
    };
    
    class VulkanRenderer final : public Renderer
    {
    public:
        explicit VulkanRenderer(HWND handle);
        ~VulkanRenderer() override;

        void Render() override;
        void Present() override;
        void FrameBufferResizeCallback(unsigned int width, unsigned int height) override;

    private:
        void CheckLayerAndExtension();
        void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);
        void CreateInstance();
        void SetupDebugMessenger();
        void CreateSurface(HWND handle);
        void SelectPhysicalDevice();
        bool IsDeviceSuitable(VkPhysicalDevice device);
        void CreateLogicalDevice();
        void CreateSwapChain();
        VkSurfaceFormatKHR ChooseSwapSurfaceFormat();
        VkPresentModeKHR ChooseSwapPresentMode();
        void CleanupSwapChain();
        void CreateImageViews();
        void CreateRenderPass();
        void CreateGraphicsPipeline();
        VkShaderModule CreateShaderModule(const std::vector<char>& code);
        void CreateFrameBuffers();
        void CreateCommandPool();
        void CreateCommandBuffer();
        void RecordCommandBuffer(uint32_t commandIndex, uint32_t imageIndex);
        void CreateSyncObjects();
        void DrawFrame();
        void RecreateSwapChain();

        // 名称比较器，按字典序排列
        constexpr static auto NameComparer = [](char const* lhs, char const* rhs) {
            return std::strcmp(lhs, rhs) < 0;
        };
        // 扩展投影
        constexpr static auto ExtensionProjector = [](const VkExtensionProperties& extensionProperties) {
            return extensionProperties.extensionName;
        };
        // 层投影
        constexpr static auto LayerProjector = [](const VkLayerProperties& layerProperties) {
            return layerProperties.layerName;
        };

        bool m_enableValidationLayer { true };
        std::vector<char const*> m_requiredVulkanExtensions;
        std::vector<char const*> m_requiredVulkanLayers;
        VkInstance m_instance{ VK_NULL_HANDLE };
        VkDebugUtilsMessengerEXT m_debugMessenger{};
        VkSurfaceKHR m_surface{};

        std::vector<char const*> m_requiredDeviceExtensions;
        std::vector<VkQueueType> m_requiredQueueFamilies;
        VkPhysicalDeviceFeatures m_deviceFeatures{};
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

        VkRenderPass m_renderPass{ VK_NULL_HANDLE };
        VkPipelineLayout m_pipelineLayout{ VK_NULL_HANDLE };
        VkPipeline m_graphicsPipeline{ VK_NULL_HANDLE };
        std::vector<VkFramebuffer> m_swapChainFrameBuffers;

        const uint32_t MAX_FRAMES_IN_FLIGHT = 3;
        VkCommandPool m_cmdPool{ VK_NULL_HANDLE };
        std::vector<VkCommandBuffer> m_cmdBuffers;

        std::vector<VkSemaphore> m_imageAvailableSemaphores;
        std::vector<VkSemaphore> m_renderFinishedSemaphores;
        std::vector<VkFence> m_inFlightFences;
        uint32_t m_currentFrame { 0 };
        uint32_t m_imageIndex{ 0 };

        bool m_framebufferResized{ false };
        unsigned int m_width{ 0 };
        unsigned int m_height{ 0 };
    };
}
