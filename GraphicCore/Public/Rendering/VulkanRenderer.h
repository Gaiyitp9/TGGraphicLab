/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Renderer.h"
#include "Base/Window.h"
#include "vulkan/vulkan.h"

namespace TG
{
    enum class VkQueueType : unsigned char
    {
        Graphic = VK_QUEUE_GRAPHICS_BIT,
        Compute = VK_QUEUE_COMPUTE_BIT,
        Transfer = VK_QUEUE_TRANSFER_BIT,
        SparseBinding = VK_QUEUE_SPARSE_BINDING_BIT,
        Present,
    };

    class VulkanRenderer final : public Renderer
    {
    public:
        explicit VulkanRenderer(NativeWindowHandle handle);
        ~VulkanRenderer() override;

        void Render() override;
        void Present() override;

    private:
        void CheckExtensionAndLayer();
        void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);
        void CreateInstance(NativeWindowHandle handle);
        void SetupDebugMessenger();
        void SelectPhysicalDevice();
        bool IsDeviceSuitable(VkPhysicalDevice device);
        void CreateLogicalDevice();

        bool m_enableValidationLayer { true };
        std::vector<char const*> m_globalExtensions;    // 需要使用到的vulkan扩展
        std::vector<char const*> m_globalLayers;        // 需要使用到的vulkan layers
        VkInstance m_instance{ VK_NULL_HANDLE };
        VkDebugUtilsMessengerEXT m_debugMessenger{};
        VkSurfaceKHR m_surface{};

        std::vector<char const*> m_deviceExtensions;
        std::vector<VkQueueType> m_queueFamilies;
        VkPhysicalDevice m_physicalDevice{ VK_NULL_HANDLE };
        std::unordered_map<VkQueueType, uint32_t> m_familyIndices;

        VkDevice m_device{ VK_NULL_HANDLE };
        std::unordered_map<VkQueueType, VkQueue> m_queues;
    };
}
