/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Rendering/VulkanRenderer.h"

#include <ranges>

#include "Diagnostic/Log.h"
#include "Exception/BaseException.h"
#include "vulkan/vulkan_win32.h"
#include <unordered_set>

namespace TG
{
    VulkanRenderer::VulkanRenderer(NativeWindowHandle handle)
    {
        CheckExtensionAndLayer();
        CreateInstance(handle);
        SetupDebugMessenger();
        SelectPhysicalDevice();
        CreateLogicalDevice();
    }

    VulkanRenderer::~VulkanRenderer()
    {
        if (m_enableValidationLayer)
        {
            const auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
                vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT"));
            if (func != nullptr)
                func(m_instance, m_debugMessenger, nullptr);
        }

        vkDestroyDevice(m_device, nullptr);
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
        vkDestroyInstance(m_instance, nullptr);
    }

    void VulkanRenderer::Render()
    {

    }

    void VulkanRenderer::Present()
    {

    }

    void VulkanRenderer::CheckExtensionAndLayer()
    {
        // 加入安装了多个版本的vulkan，指定版本需要设置
        // 1. VK_LAYER_PATH=/path/to/vulkan/Bin来寻找VkLayer_khronos_validation.dll
        // 2. VK_SDK_PATH=/path/to/vulkan/Bin
        // 3. VULKAN_SDK=/path/to/vulkan/Bin
        // Windows平台上，也可以在注册表中的下面两项中添加*.json文件路径
        // HKEY_LOCAL_MACHINE\SOFTWARE\Khronos\Vulkan\ExplicitLayers
        // HKEY_LOCAL_MACHINE\SOFTWARE\Khronos\Vulkan\ImplicitLayers

        if (m_enableValidationLayer)
            m_globalLayers.emplace_back("VK_LAYER_KHRONOS_validation");

        // 查询可用的layer
        uint32_t layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
        std::string layersLog;
        for (const auto& layerProperties : availableLayers)
        {
            layersLog += layerProperties.layerName;
            layersLog += "\n";
        }
        LogInfo("Available layers:\n{}", layersLog);

        // 检查是否包含所需的layer
        for (char const* layerName : m_globalLayers)
        {
            bool layerFound = false;
            for (const auto& layerProperties : availableLayers)
            {
                if (std::strcmp(layerName, layerProperties.layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound)
                throw BaseException::Create("Layers required, but not available");
        }

        m_globalExtensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);
        m_globalExtensions.emplace_back("VK_KHR_win32_surface");
        if (m_enableValidationLayer)
            m_globalExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());
        std::string extensionsLog;
        for (const auto& extensionProperties : availableExtensions)
        {
            extensionsLog += extensionProperties.extensionName;
            extensionsLog += "\n";
        }
        LogInfo("Available extensions:\n{}", extensionsLog);

        // 检查是否包含所需的extension
        for (char const* extensionName : m_globalExtensions)
        {
            bool extensionFound = false;
            for (const auto& extensionProperties : availableExtensions)
            {
                if (std::strcmp(extensionName, extensionProperties.extensionName) == 0)
                {
                    extensionFound = true;
                    break;
                }
            }

            if (!extensionFound)
                throw BaseException::Create("Extensions required, but not available");
        }
    }

    void VulkanRenderer::PopulateDebugMessengerCreateInfo(
        VkDebugUtilsMessengerCreateInfoEXT& createInfo)
    {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = DebugCallback;
        createInfo.pUserData = nullptr;
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL VulkanRenderer::DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        LogError("validation layer: {}", pCallbackData->pMessage);
        // The callback returns a boolean that indicates if the Vulkan call that
        // triggered the validation layer message should be aborted. If the callback
        // returns true, then the call is aborted with the VK_ERROR_VALIDATION_FAILED_EXT error.
        // This is normally only used to test the validation layers themselves, so you should always return VK_FALSE.
        return VK_FALSE;
    }

    void VulkanRenderer::CreateInstance(NativeWindowHandle handle)
    {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "TG Renderer";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_4;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(m_globalExtensions.size());
        createInfo.ppEnabledExtensionNames = m_globalExtensions.data();
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (m_enableValidationLayer)
        {
            PopulateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.enabledLayerCount = static_cast<uint32_t>(m_globalLayers.size());
            createInfo.ppEnabledLayerNames = m_globalLayers.data();
            createInfo.pNext = &debugCreateInfo;
        }

        if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
            throw BaseException::Create("Failed to create instance");

        VkWin32SurfaceCreateInfoKHR surfaceCreateInfo{};
        surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        surfaceCreateInfo.hwnd = handle;
        surfaceCreateInfo.hinstance = GetModuleHandle(nullptr);
        if (vkCreateWin32SurfaceKHR(m_instance, &surfaceCreateInfo, nullptr, &m_surface)
            != VK_SUCCESS)
        {
            throw BaseException::Create("Failed to create window surface");
        }
    }

    void VulkanRenderer::SetupDebugMessenger()
    {
        if (!m_enableValidationLayer)
            return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        PopulateDebugMessengerCreateInfo(createInfo);
        const auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT"));
        if (func == nullptr)
            throw BaseException::Create("Failed to set up debug messenger");
        func(m_instance, &createInfo, nullptr, &m_debugMessenger);
    }

    void VulkanRenderer::SelectPhysicalDevice()
    {
        m_deviceExtensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        m_queueFamilies.emplace_back(VkQueueType::Graphic);
        m_queueFamilies.emplace_back(VkQueueType::Compute);
        m_queueFamilies.emplace_back(VkQueueType::Transfer);
        m_queueFamilies.emplace_back(VkQueueType::Present);

        // 获取所有显示设备
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
        if (deviceCount == 0)
            throw BaseException::Create("Failed to find GPUs with Vulkan support");
        std::vector<VkPhysicalDevice> availableDevices(deviceCount);
        vkEnumeratePhysicalDevices(m_instance, &deviceCount, availableDevices.data());
        // 打印所有显示设备信息
        for (const auto& device : availableDevices)
        {
            VkPhysicalDeviceProperties deviceProperties{};
            vkGetPhysicalDeviceProperties(device, &deviceProperties);
            LogInfo("Device type: {} Device name: {}",
                std::to_underlying(deviceProperties.deviceType),
                deviceProperties.deviceName);
        }

        // 选择合适的显示设备
        for (const auto& device : availableDevices)
        {
            if (IsDeviceSuitable(device))
            {
                m_physicalDevice = device;
                break;
            }
        }
        if (m_physicalDevice == VK_NULL_HANDLE)
            throw BaseException::Create("Failed to find a suitable GPU");
    }

    bool VulkanRenderer::IsDeviceSuitable(VkPhysicalDevice device)
    {
        // 获取显示设备支持的扩展
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
            availableExtensions.data());
        VkPhysicalDeviceProperties deviceProperties{};
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        std::string deviceExtensionsLog;
        for (const auto& extensionProperties : availableExtensions)
        {
            deviceExtensionsLog += extensionProperties.extensionName;
            deviceExtensionsLog += "\n";
        }
        LogInfo("{} extensions:\n{}", deviceProperties.deviceName, deviceExtensionsLog);

        // 检查是否包含所需扩展
        for (char const* extension : m_deviceExtensions)
        {
            bool extensionFound = false;
            for (const auto& extensionProperties : availableExtensions)
            {
                if (std::strcmp(extensionProperties.extensionName, extension) == 0)
                {
                    extensionFound = true;
                    break;
                }
            }
            if (!extensionFound)
                return false;
        }

        // 获取设备支持的queue family
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> availableQueueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
            availableQueueFamilies.data());
        for (const auto& queueFamily : m_queueFamilies)
        {
            bool familyFound = false;
            for (std::size_t i = 0; i < availableQueueFamilies.size(); ++i)
            {
                if (availableQueueFamilies[i].queueFlags & static_cast<VkQueueFlags>(queueFamily))
                {
                    m_familyIndices[queueFamily] = static_cast<uint32_t>(i);
                    familyFound = true;
                    break;
                }

                // 是否支持present
                VkBool32 presentSupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(device, static_cast<uint32_t>(i),
                    m_surface, &presentSupport);
                if (presentSupport)
                {
                    m_familyIndices[VkQueueType::Present] = static_cast<uint32_t>(i);
                    familyFound = true;
                    break;
                }
            }
            if (!familyFound)
            {
                m_familyIndices.clear();
                return false;
            }
        }

        // 检查是否支持swapchain
        uint32_t formatCount = 0;
        uint32_t presentModeCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, nullptr);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, nullptr);
        if (formatCount == 0 || presentModeCount == 0)
            return false;

        VkPhysicalDeviceFeatures supportedFeatures{};
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
        if (!supportedFeatures.samplerAnisotropy)
            return false;

        return true;
    }

    void VulkanRenderer::CreateLogicalDevice()
    {
        // 保证每个queue family是唯一的
        std::unordered_set<uint32_t> uniqueQueueFamilies;
        for (const auto &index: m_familyIndices | std::views::values)
            uniqueQueueFamilies.insert(index);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        constexpr float queuePriority = 1.0f;
        for (const auto& queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.emplace_back(queueCreateInfo);
        }

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        // 启用各向异性采样
        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(m_deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = m_deviceExtensions.data();
        if (m_enableValidationLayer)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(m_globalLayers.size());
            createInfo.ppEnabledLayerNames = m_globalLayers.data();
        }

        if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS)
            throw BaseException::Create("Failed to create logical device!");

        for (const auto& pair : m_familyIndices)
        {
            VkQueue queue;
            vkGetDeviceQueue(m_device, pair.second, 0, &queue);
            m_queues[pair.first] = queue;
        }
    }
}
