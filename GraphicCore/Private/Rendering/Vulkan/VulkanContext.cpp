/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Rendering/Vulkan/VulkanContext.h"
#include "Base/Utility.h"
#include "Diagnostic/Log.hpp"
#include "Exception/BaseException.h"
#include "vulkan/vulkan_win32.h"
#include <ranges>
#include <unordered_set>

namespace TG::Rendering
{
	VulkanContext::VulkanContext(const IDefaultVideoPort& videoPort)
	{
		CheckLayerAndExtension();
		CreateInstance();
		SetupDebugMessenger();
		CreateSurface(videoPort.Handle());
		SelectPhysicalDevice();
		CreateLogicalDevice();
		CreateSwapChain();
		CreateImageViews();
	}

	VulkanContext::~VulkanContext()
	{
		if (m_instance == VK_NULL_HANDLE || m_device == VK_NULL_HANDLE)
			return;

		vkDeviceWaitIdle(m_device);

		CleanupSwapChain();

		vkDestroyDevice(m_device, nullptr);
		vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
		if (m_enableValidationLayer)
		{
			const auto vkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
				vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT"));
			if (vkDestroyDebugUtilsMessengerEXT != nullptr)
				vkDestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
		}
		vkDestroyInstance(m_instance, nullptr);
	}

	VkDevice VulkanContext::GetDevice() const
	{
		return m_device;
	}

	VkFormat VulkanContext::GetSwapChainImageFormat() const
	{
		return m_swapChainImageFormat;
	}

	const std::vector<VkImageView>& VulkanContext::GetSwapChainImageViews() const
	{
		return m_swapChainImageViews;
	}

	VkExtent2D VulkanContext::GetSwapChainExtent() const
	{
		return m_swapChainExtent;
	}

	std::unordered_map<VkQueueType, uint32_t> VulkanContext::GetQueueFamilyIndices() const
	{
		return m_familyIndices;
	}

	void VulkanContext::CheckLayerAndExtension()
	{
		// 假如安装了多个版本的vulkan，指定版本需要设置
        // 1. VK_LAYER_PATH=/path/to/vulkan/Bin来寻找VkLayer_khronos_validation.dll
        // 2. VK_SDK_PATH=/path/to/vulkan/Bin
        // 3. VULKAN_SDK=/path/to/vulkan/Bin
        // Windows平台上，也可以在注册表中的下面两项中添加*.json文件路径
        // HKEY_LOCAL_MACHINE\SOFTWARE\Khronos\Vulkan\ExplicitLayers
        // HKEY_LOCAL_MACHINE\SOFTWARE\Khronos\Vulkan\ImplicitLayers

        // 需要开启的层
        if (m_enableValidationLayer)
            m_requiredVulkanLayers.emplace_back("VK_LAYER_KHRONOS_validation");
        std::ranges::sort(m_requiredVulkanLayers, NameComparer);
        // 查询可用的层
        uint32_t layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
        std::ranges::sort(availableLayers, NameComparer, LayerProjector);
        // 可用层输出到日志
        std::string layersLog;
        for (const auto& layerProperties : availableLayers)
        {
            layersLog += layerProperties.layerName;
            layersLog += "\n";
        }
        LogInfo("\nAvailable layers:\n{}", layersLog);
        // 检查是否包含所需层
        const bool layerFound = std::ranges::includes(availableLayers, m_requiredVulkanLayers,
            NameComparer, LayerProjector);
        if (!layerFound)
            throw BaseException::Create("Layers required, but not available");

        // 需要支持的扩展
        m_requiredVulkanExtensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);
        m_requiredVulkanExtensions.emplace_back("VK_KHR_win32_surface");
        if (m_enableValidationLayer)
            m_requiredVulkanExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        std::ranges::sort(m_requiredVulkanExtensions, NameComparer);
        // 查询可用的扩展
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());
        std::ranges::sort(availableExtensions, NameComparer, ExtensionProjector);
        // 可用扩展输出到日志
        std::string extensionsLog;
        for (const auto& extensionProperties : availableExtensions)
        {
            extensionsLog += extensionProperties.extensionName;
            extensionsLog += "\n";
        }
        LogInfo("\nAvailable extensions:\n{}", extensionsLog);
        // 检查是否包含所需扩展
        const bool extensionFound = std::ranges::includes(availableExtensions, m_requiredVulkanExtensions,
            NameComparer, ExtensionProjector);
        if (!extensionFound)
            throw BaseException::Create("Extensions required, but not available");
	}

    void VulkanContext::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
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

	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanContext::DebugCallback(
			[[maybe_unused]] VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			[[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			[[maybe_unused]] void* pUserData
	)
	{
		LogInfo("validation layer: {}", pCallbackData->pMessage);
		// The callback returns a boolean that indicates if the Vulkan call that
		// triggered the validation layer message should be aborted. If the callback
		// returns true, then the call is aborted with the VK_ERROR_VALIDATION_FAILED_EXT error.
		// This is normally only used to test the validation layers themselves, so you should always return VK_FALSE.
		return VK_FALSE;
	}

	void VulkanContext::CreateInstance()
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "TG Graphic Lab";
		appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_4;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(m_requiredVulkanExtensions.size());
		createInfo.ppEnabledExtensionNames = m_requiredVulkanExtensions.data();
		createInfo.enabledLayerCount = static_cast<uint32_t>(m_requiredVulkanLayers.size());
		createInfo.ppEnabledLayerNames = m_requiredVulkanLayers.data();
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (m_enableValidationLayer)
		{
			PopulateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = &debugCreateInfo;
		}

		if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
			throw BaseException::Create("Failed to create instance");
	}

	void VulkanContext::SetupDebugMessenger()
	{
		if (!m_enableValidationLayer)
			return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		PopulateDebugMessengerCreateInfo(createInfo);
		const auto vkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
			vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT"));
		if (vkCreateDebugUtilsMessengerEXT == nullptr)
			throw BaseException::Create("Failed to set up debug messenger");
		vkCreateDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_debugMessenger);
	}

	void VulkanContext::CreateSurface(HWND handle)
	{
		VkWin32SurfaceCreateInfoKHR surfaceCreateInfo{};
		surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceCreateInfo.hwnd = handle;
		surfaceCreateInfo.hinstance = GetModuleHandle(nullptr);
		if (vkCreateWin32SurfaceKHR(m_instance, &surfaceCreateInfo, nullptr, &m_surface) != VK_SUCCESS)
			throw BaseException::Create("Failed to create window surface");
	}

	void VulkanContext::SelectPhysicalDevice()
	{
		// 设备需要支持的扩展
		m_requiredDeviceExtensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		std::ranges::sort(m_requiredVulkanExtensions, NameComparer);
		// 队列族需要支持的队列类型
		m_requiredQueueFamilies.emplace_back(VkQueueType::Graphic);
		m_requiredQueueFamilies.emplace_back(VkQueueType::Compute);
		m_requiredQueueFamilies.emplace_back(VkQueueType::Transfer);
		m_requiredQueueFamilies.emplace_back(VkQueueType::Present);
		// 设备需要支持的特性
		m_deviceFeatures.samplerAnisotropy = VK_TRUE;

		// 获取所有显示设备
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
		if (deviceCount == 0)
			throw BaseException::Create("Failed to find GPUs with Vulkan support");
		std::vector<VkPhysicalDevice> availableDevices(deviceCount);
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, availableDevices.data());
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

	bool VulkanContext::IsDeviceSuitable(VkPhysicalDevice device)
	{
		// 获取设备属性
        VkPhysicalDeviceProperties deviceProperties{};
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        // 获取显示设备支持的扩展
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
        std::ranges::sort(availableExtensions, NameComparer, ExtensionProjector);
        // 设备扩展输出到日志
        std::string deviceExtensionsLog;
        for (const auto& extensionProperties : availableExtensions)
        {
            deviceExtensionsLog += extensionProperties.extensionName;
            deviceExtensionsLog += "\n";
        }
        LogInfo("\nDevice type: {} Device name: {}\nExtensions:\n{}",
                std::to_underlying(deviceProperties.deviceType),
                deviceProperties.deviceName,
                deviceExtensionsLog);
        // 检查是否包含所需扩展
        const bool extensionFound = std::ranges::includes(availableExtensions, m_requiredDeviceExtensions,
            NameComparer, ExtensionProjector);
        if (!extensionFound)
            return false;

        // 获取设备支持的队列族
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> availableQueueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, availableQueueFamilies.data());
        // 检查是否包含所需的队列族
        for (const auto& queueFamily : m_requiredQueueFamilies)
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

        // 检查设备是否支持所需特性
        VkPhysicalDeviceFeatures supportedFeatures{};
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
        if (m_deviceFeatures.samplerAnisotropy && !supportedFeatures.samplerAnisotropy)
            return false;

        return true;
	}

	void VulkanContext::CreateLogicalDevice()
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
		createInfo.pEnabledFeatures = &m_deviceFeatures;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(m_requiredDeviceExtensions.size());
		createInfo.ppEnabledExtensionNames = m_requiredDeviceExtensions.data();
		if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS)
			throw BaseException::Create("Failed to create logical device!");

		for (const auto& pair : m_familyIndices)
		{
			VkQueue queue;
			vkGetDeviceQueue(m_device, pair.second, 0, &queue);
			m_queues[pair.first] = queue;
		}
	}

	void VulkanContext::CreateSwapChain()
	{
		// 获取surface属性
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
        m_swapChainImageFormat = surfaceFormat.format;
        m_swapChainExtent = m_capabilities.currentExtent;

        uint32_t imageCount = m_capabilities.minImageCount + 1;
        if (m_capabilities.maxImageCount > 0 && imageCount > m_capabilities.maxImageCount)
            imageCount = m_capabilities.maxImageCount;

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = m_swapChainImageFormat;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = m_swapChainExtent;
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
	}

	VkSurfaceFormatKHR VulkanContext::ChooseSwapSurfaceFormat()
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

	VkPresentModeKHR VulkanContext::ChooseSwapPresentMode()
	{
		// for (const auto& availablePresentMode : m_presentModes)
		// {
		//     if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		//         return availablePresentMode;
		// }

		// 开启垂直同步
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	void VulkanContext::CleanupSwapChain()
	{
		for (const auto& swapChainImageView : m_swapChainImageViews)
			vkDestroyImageView(m_device, swapChainImageView, nullptr);

		for (const auto& swapChainImageView : m_swapChainImageViews)
			vkDestroyImageView(m_device, swapChainImageView, nullptr);

		vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);
	}

	void VulkanContext::CreateImageViews()
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

	void VulkanContext::RecreateSwapChain()
	{
		vkDeviceWaitIdle(m_device);

		CleanupSwapChain();

		CreateSwapChain();
		CreateImageViews();
	}
}
