/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Rendering/VulkanRenderer.h"
#include "Base/Utility.h"
#include "Diagnostic/Log.h"
#include "Exception/BaseException.h"
#include "vulkan/vulkan_win32.h"
#include <ranges>

namespace TG
{
    VulkanRenderer::VulkanRenderer(NativeWindowHandle handle)
    {
        CheckExtensionAndLayer();
        CreateInstance(handle);
        SetupDebugMessenger();
        SelectPhysicalDevice();
        CreateLogicalDevice();
        CreateSwapChain();
        CreateImageViews();
        CreateRenderPass();
        CreateGraphicsPipeline();
        CreateCommandPool();
        CreateCommandBuffer();
        CreateFramebuffers();
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

        CleanupSwapChain();

        vkDestroyPipeline(m_device, m_graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
        vkDestroyRenderPass(m_device, m_renderPass, nullptr);
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
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;
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
        if (vkCreateWin32SurfaceKHR(m_instance, &surfaceCreateInfo, nullptr, &m_surface) != VK_SUCCESS)
            throw BaseException::Create("Failed to create window surface");
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

        // 检查交换链格式和present模式
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
        m_deviceFeatures.samplerAnisotropy = VK_TRUE;

        return true;
    }

    void VulkanRenderer::CreateLogicalDevice()
    {
        std::unordered_set<uint32_t> uniqueQueueFamilies;
        // 保证每个queue family是唯一的
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
        createInfo.pEnabledFeatures = &m_deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(m_deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = m_deviceExtensions.data();
        if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS)
            throw BaseException::Create("Failed to create logical device!");

        for (const auto& pair : m_familyIndices)
        {
            VkQueue queue;
            vkGetDeviceQueue(m_device, pair.second, 0, &queue);
            m_queues[pair.first] = queue;
        }
    }

    void VulkanRenderer::CreateSwapChain()
    {
        // 获取支持的交换链属性
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &m_capabilities);
        // 获取支持的交换链格式
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, nullptr);
        m_swapChainFormats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount,
            m_swapChainFormats.data());
        // 获取支持的交换链present模式
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, nullptr);
        m_presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount,
            m_presentModes.data());

        VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat();
        VkPresentModeKHR presentMode = ChooseSwapPresentMode();
        VkExtent2D extent = m_capabilities.currentExtent;

        uint32_t imageCount = m_capabilities.minImageCount + 1;
        if (m_capabilities.maxImageCount > 0 && imageCount > m_capabilities.maxImageCount)
            imageCount = m_capabilities.maxImageCount;

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        uint32_t presentGraphicFamilyIndices[] = {
            m_familyIndices[VkQueueType::Present],
            m_familyIndices[VkQueueType::Graphic],
        };
        if (m_familyIndices[VkQueueType::Graphic] != m_familyIndices[VkQueueType::Present])
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = presentGraphicFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }
        createInfo.preTransform = m_capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = m_oldSwapChain;

        if (vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
            throw BaseException::Create("Failed to create swap chain!");

        vkGetSwapchainImagesKHR(m_device, m_swapChain, &imageCount, nullptr);
        m_swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(m_device, m_swapChain, &imageCount, m_swapChainImages.data());
        m_swapChainImageFormat = surfaceFormat.format;
        m_swapChainExtent = extent;
    }

    VkSurfaceFormatKHR VulkanRenderer::ChooseSwapSurfaceFormat()
    {
        for (const auto& availableFormat : m_swapChainFormats)
        {
            if (availableFormat.format == VK_FORMAT_R8G8B8A8_SRGB &&
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }

        return m_swapChainFormats[0];
    }

    VkPresentModeKHR VulkanRenderer::ChooseSwapPresentMode()
    {
        for (const auto& availablePresentMode : m_presentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
                return availablePresentMode;
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    void VulkanRenderer::CleanupSwapChain()
    {
        for (const auto& framebuffer : m_swapChainFrameBuffers)
            vkDestroyFramebuffer(m_device, framebuffer, nullptr);

        for (const auto& swapChainImageView : m_swapChainImageViews)
            vkDestroyImageView(m_device, swapChainImageView, nullptr);

        vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);
    }

    void VulkanRenderer::CreateImageViews()
    {
        m_swapChainImageViews.resize(m_swapChainImages.size());
        for (std::size_t i = 0; i < m_swapChainImages.size(); ++i)
        {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = m_swapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = m_swapChainImageFormat;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;
            if (vkCreateImageView(m_device, &createInfo, nullptr, &m_swapChainImageViews[i]) != VK_SUCCESS)
                throw BaseException::Create("Failed to create image view!");
        }
    }

    void VulkanRenderer::CreateRenderPass()
    {
        VkAttachmentDescription colorAttachments{};
        colorAttachments.format = m_swapChainImageFormat;
        colorAttachments.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachments.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachments.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachments.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachments.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachments.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachments.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachments;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;

        if (vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS)
            throw BaseException::Create("Failed to create render pass!");
    }

    void VulkanRenderer::CreateGraphicsPipeline()
    {
        // 创建顶点着色器
        std::vector<char> vertShaderCode = ReadBinaryFile("../../Shaders/GLSL/first_vert.spv");
        VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);
        VkPipelineShaderStageCreateInfo vertShaderStageCreateInfo = {};
        vertShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageCreateInfo.module = vertShaderModule;
        vertShaderStageCreateInfo.pName = "main";
        // 创建片元着色器
        std::vector<char> fragShaderCode = ReadBinaryFile("../../Shaders/GLSL/first_frag.spv");
        VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);
        VkPipelineShaderStageCreateInfo fragShaderStageCreateInfo = {};
        fragShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageCreateInfo.module = fragShaderModule;
        fragShaderStageCreateInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageCreateInfo, fragShaderStageCreateInfo };

        // 顶点输入状态
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexBindingDescriptions = nullptr;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr;
        // 顶点装配器
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
        inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
        // 视口和裁剪
        // VkViewport viewport{};
        // viewport.x = 0.0f;
        // viewport.y = 0.0f;
        // viewport.width = static_cast<float>(m_swapChainExtent.width);
        // viewport.height = static_cast<float>(m_swapChainExtent.height);
        // viewport.minDepth = 0.0f;
        // viewport.maxDepth = 1.0f;
        // VkRect2D scissor{};
        // scissor.offset = { 0, 0 };
        // scissor.extent = m_swapChainExtent;
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
        rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
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
        pipelineLayoutCreateInfo.setLayoutCount = 0;
        pipelineLayoutCreateInfo.pSetLayouts = nullptr;
        pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
        pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;
        if (vkCreatePipelineLayout(m_device, &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
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
        pipelineCreateInfo.renderPass = m_renderPass;
        pipelineCreateInfo.subpass = 0;
        pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineCreateInfo.basePipelineIndex = -1;
        if (vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr,
            &m_graphicsPipeline) != VK_SUCCESS)
        {
            throw BaseException::Create("Failed to create graphics pipeline!");
        }

        vkDestroyShaderModule(m_device, vertShaderModule, nullptr);
        vkDestroyShaderModule(m_device, fragShaderModule, nullptr);
    }

    VkShaderModule VulkanRenderer::CreateShaderModule(const std::vector<char>& code)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
        VkShaderModule shaderModule;
        if (vkCreateShaderModule(m_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
            throw BaseException::Create("Failed to create shader module!");
        return shaderModule;
    }

    void VulkanRenderer::CreateFramebuffers()
    {
        m_swapChainFrameBuffers.resize(m_swapChainImageViews.size());
        for (std::size_t i = 0; i < m_swapChainImageViews.size(); ++i)
        {
            VkImageView attachments[] = { m_swapChainImageViews[i] };

            VkFramebufferCreateInfo framebufferCreateInfo{};
            framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo.renderPass = m_renderPass;
            framebufferCreateInfo.attachmentCount = 1;
            framebufferCreateInfo.pAttachments = attachments;
            framebufferCreateInfo.width = m_swapChainExtent.width;
            framebufferCreateInfo.height = m_swapChainExtent.height;
            framebufferCreateInfo.layers = 1;
            if (vkCreateFramebuffer(m_device, &framebufferCreateInfo, nullptr,
                &m_swapChainFrameBuffers[i]) != VK_SUCCESS)
            {
                throw BaseException::Create("Failed to create framebuffer!");
            }
        }
    }

    void VulkanRenderer::CreateCommandPool()
    {

    }

    void VulkanRenderer::CreateCommandBuffer()
    {
        
    }
}
