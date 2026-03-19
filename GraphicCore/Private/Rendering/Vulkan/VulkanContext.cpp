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
		: m_videoPort{ videoPort }
	{
		CheckLayerAndExtension();
		CreateInstance();
		SetupDebugMessenger();
		CreateSurface(videoPort.Handle());
		SelectPhysicalDevice();
		CreateLogicalDevice();
		CreateSwapChain();
		CreateCommandPool();
		CreateCommandBuffers();
		CreateSyncObjects();
	}

	VulkanContext::~VulkanContext()
	{
		if (m_instance == VK_NULL_HANDLE || m_device == VK_NULL_HANDLE)
			return;

		vkDeviceWaitIdle(m_device);

		for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			vkDestroySemaphore(m_device, m_imageAvailableSemaphores[i], nullptr);
			vkDestroySemaphore(m_device, m_renderFinishedSemaphores[i], nullptr);
			vkDestroyFence(m_device, m_inFlightFences[i], nullptr);
		}

        vkDestroyCommandPool(m_device, m_cmdPool, nullptr);

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

	const IDefaultVideoPort& VulkanContext::VideoPort() const
	{
		return m_videoPort;
	}

	VkInstance VulkanContext::GetInstance() const
	{
		return m_instance;
	}

	VkDevice VulkanContext::GetDevice() const
	{
		return m_device;
	}

	VkPhysicalDevice VulkanContext::GetPhysicalDevice() const
	{
		return m_physicalDevice;
	}

	uint32_t VulkanContext::GetQueueFamily(VkQueueType type) const
	{
		return m_familyIndices.at(type);
	}

	VkQueue VulkanContext::GetQueue(VkQueueType type) const
	{
		return m_queues.at(type);
	}

	VkFormat VulkanContext::GetSwapChainFormat() const
	{
		return m_swapChainImageFormat;
	}

	uint32_t VulkanContext::GetSwapChainMinImageCount() const
	{
		return m_capabilities.minImageCount;
	}

	uint32_t VulkanContext::GetSwapChainImageCount() const
	{
		return static_cast<uint32_t>(m_swapChainImages.size());
	}

	const std::vector<VkImage>& VulkanContext::GetSwapChainImages() const
	{
		return m_swapChainImages;
	}

	const std::vector<VkImageView>& VulkanContext::GetSwapChainImageViews() const
	{
		return m_swapChainImageViews;
	}

	VkExtent2D VulkanContext::GetSwapChainExtent() const
	{
		return m_swapChainExtent;
	}

	uint32_t VulkanContext::GetCurrentImageIndex() const
	{
		return m_imageIndex;
	}

	VkCommandPool VulkanContext::GetCommandPool() const
	{
		return m_cmdPool;
	}

	VkCommandBuffer VulkanContext::GetCommandBuffer() const
	{
		return m_cmdBuffers[m_currentFrame];
	}

	uint32_t VulkanContext::GetMaxFramesInFlight() const
	{
		return MAX_FRAMES_IN_FLIGHT;
	}

	uint32_t VulkanContext::GetCurrentFrameIndex() const
	{
		return m_currentFrame;
	}

	bool VulkanContext::AcquireNextImage()
	{
		vkWaitForFences(m_device, 1, &m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);

		VkResult result = vkAcquireNextImageKHR(m_device, m_swapChain, UINT64_MAX,
			m_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &m_imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapChain();
			return false;
		}
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			throw BaseException::Create("Failed to acquire image!");
		// 放在这里防止死锁，因为vkAcquireNextImageKHR返回VK_ERROR_OUT_OF_DATE_KHR后会直接返回，
		// 导致Fence不会激活，所以确保获取成功后再重置Fence
		vkResetFences(m_device, 1, &m_inFlightFences[m_currentFrame]);

		return true;
	}

	void VulkanContext::BeginCommandBuffer() const
	{
		// 重置命令缓冲
		vkResetCommandBuffer(m_cmdBuffers[m_currentFrame], 0);

		// 记录绘制命令
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0;
		beginInfo.pInheritanceInfo = nullptr;
		if (vkBeginCommandBuffer(m_cmdBuffers[m_currentFrame], &beginInfo) != VK_SUCCESS)
			throw BaseException::Create("Failed to begin recording command buffer!");
	}

	void VulkanContext::EndCommandBuffer() const
	{
		if (vkEndCommandBuffer(m_cmdBuffers[m_currentFrame]) != VK_SUCCESS)
			throw BaseException::Create("Failed to record command buffer!");
	}

	void VulkanContext::Submit()
	{
		// 提交绘制命令
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_cmdBuffers[m_currentFrame];
		VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphores[m_currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphores[m_currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;
		if (vkQueueSubmit(m_queues[VkQueueType::Graphic], 1, &submitInfo,
				m_inFlightFences[m_currentFrame]) != VK_SUCCESS)
		{
			throw BaseException::Create("Failed to submit command buffer!");
		}
	}

	bool VulkanContext::Present()
	{
		bool success = true;

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphores[m_currentFrame] };
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		VkSwapchainKHR swapChains[] = { m_swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &m_imageIndex;
		presentInfo.pResults = nullptr;
		VkResult result = vkQueuePresentKHR(m_queues[VkQueueType::Present], &presentInfo);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			RecreateSwapChain();
			success = false;
		}
		else if (result != VK_SUCCESS)
			throw std::runtime_error("failed to present swap chain image");

		m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

		return success;
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

		auto nameComparer = [](char const* lhs, char const* rhs) {
			return std::strcmp(lhs, rhs) < 0;
		};
		auto layerProjector = [](const VkLayerProperties& layerProperties) {
				return layerProperties.layerName;
		};
		auto extensionProjector = [](const VkExtensionProperties& extensionProperties) {
				return extensionProperties.extensionName;
		};

        if (m_enableValidationLayer)
            m_requiredVulkanLayers.emplace_back("VK_LAYER_KHRONOS_validation");
        std::ranges::sort(m_requiredVulkanLayers, nameComparer);
        // 查询可用的层
        uint32_t layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
        std::ranges::sort(availableLayers, nameComparer, layerProjector);
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
            nameComparer, layerProjector);
        if (!layerFound)
            throw BaseException::Create("Layers required, but not available");

        if (m_enableValidationLayer)
            m_requiredVulkanExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        std::ranges::sort(m_requiredVulkanExtensions, nameComparer);
        // 查询可用的扩展
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());
        std::ranges::sort(availableExtensions, nameComparer, extensionProjector);
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
            nameComparer, extensionProjector);
        if (!extensionFound)
            throw BaseException::Create("Extensions required, but not available");
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
			debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;
			debugCreateInfo.pfnUserCallback = DebugCallback;
			debugCreateInfo.pUserData = nullptr;
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
		auto nameComparer = [](char const* lhs, char const* rhs) {
			return std::strcmp(lhs, rhs) < 0;
		};
		auto extensionProjector = [](const VkExtensionProperties& extensionProperties) {
			return extensionProperties.extensionName;
		};

        // 获取显示设备支持的扩展
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
        std::ranges::sort(availableExtensions, nameComparer, extensionProjector);
        // 设备扩展输出到日志
        std::string deviceExtensionsLog;
        for (const auto& extensionProperties : availableExtensions)
        {
            deviceExtensionsLog += extensionProperties.extensionName;
            deviceExtensionsLog += "\n";
        }
		// 获取设备属性
		VkPhysicalDeviceProperties deviceProperties{};
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
        LogInfo("\nDevice type: {} Device name: {}\nExtensions:\n{}",
                std::to_underlying(deviceProperties.deviceType),
                deviceProperties.deviceName,
                deviceExtensionsLog);
        // 检查是否包含所需扩展
		std::ranges::sort(m_requiredDeviceExtensions, nameComparer);
        const bool extensionFound = std::ranges::includes(availableExtensions, m_requiredDeviceExtensions,
            nameComparer, extensionProjector);
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
        if (!supportedFeatures.samplerAnisotropy)
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

		// 开启动态渲染扩展
		VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamicRenderingFeature{};
		dynamicRenderingFeature.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
		dynamicRenderingFeature.dynamicRendering = VK_TRUE;

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pNext = &dynamicRenderingFeature;
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

        VkSwapchainCreateInfoKHR swapChainCreateInfo{};
        swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapChainCreateInfo.surface = m_surface;
        swapChainCreateInfo.minImageCount = imageCount;
        swapChainCreateInfo.imageFormat = m_swapChainImageFormat;
        swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
        swapChainCreateInfo.imageExtent = m_swapChainExtent;
        swapChainCreateInfo.imageArrayLayers = 1;
        swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        uint32_t presentGraphicFamilyIndices[] = {
            m_familyIndices[VkQueueType::Present],
            m_familyIndices[VkQueueType::Graphic],
        };
        if (m_familyIndices[VkQueueType::Graphic] != m_familyIndices[VkQueueType::Present])
        {
            swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            swapChainCreateInfo.queueFamilyIndexCount = 2;
            swapChainCreateInfo.pQueueFamilyIndices = presentGraphicFamilyIndices;
        }
        else
        {
            swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            swapChainCreateInfo.queueFamilyIndexCount = 0;
            swapChainCreateInfo.pQueueFamilyIndices = nullptr;
        }
        swapChainCreateInfo.preTransform = m_capabilities.currentTransform;
        swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapChainCreateInfo.presentMode = presentMode;
        swapChainCreateInfo.clipped = VK_TRUE;
        swapChainCreateInfo.oldSwapchain = m_oldSwapChain;

        if (vkCreateSwapchainKHR(m_device, &swapChainCreateInfo, nullptr, &m_swapChain) != VK_SUCCESS)
            throw BaseException::Create("Failed to create swap chain!");

        vkGetSwapchainImagesKHR(m_device, m_swapChain, &imageCount, nullptr);
        m_swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(m_device, m_swapChain, &imageCount, m_swapChainImages.data());

		m_swapChainImageViews.resize(m_swapChainImages.size());
		for (std::size_t i = 0; i < m_swapChainImages.size(); ++i)
		{
			VkImageViewCreateInfo imageViewCreateInfo{};
			imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewCreateInfo.image = m_swapChainImages[i];
			imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewCreateInfo.format = m_swapChainImageFormat;
			imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = 1;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 1;
			if (vkCreateImageView(m_device, &imageViewCreateInfo, nullptr, &m_swapChainImageViews[i]) != VK_SUCCESS)
				throw BaseException::Create("Failed to create image view!");
		}
	}

	VkSurfaceFormatKHR VulkanContext::ChooseSwapSurfaceFormat() const
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

	VkPresentModeKHR VulkanContext::ChooseSwapPresentMode() const
	{
		for (const auto& availablePresentMode : m_presentModes)
		{
		    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		        return availablePresentMode;
		}

		// 唯一强制支持的模式
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	void VulkanContext::CleanupSwapChain()
	{
		for (const auto& swapChainImageView : m_swapChainImageViews)
			vkDestroyImageView(m_device, swapChainImageView, nullptr);
		m_swapChainImageViews.clear();
		m_swapChainImages.clear();

		vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);
	}

	void VulkanContext::RecreateSwapChain()
	{
		vkDeviceWaitIdle(m_device);

		for (const auto& swapChainImageView : m_swapChainImageViews)
			vkDestroyImageView(m_device, swapChainImageView, nullptr);
		m_swapChainImageViews.clear();
		m_swapChainImages.clear();

		m_oldSwapChain = m_swapChain;

		CreateSwapChain();

		vkDestroySwapchainKHR(m_device, m_oldSwapChain, nullptr);
	}

	void VulkanContext::CreateCommandPool()
	{
		VkCommandPoolCreateInfo commandPoolCreateInfo{};
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		commandPoolCreateInfo.queueFamilyIndex = m_familyIndices[VkQueueType::Graphic];
		if (vkCreateCommandPool(m_device, &commandPoolCreateInfo, nullptr, &m_cmdPool) != VK_SUCCESS)
			throw BaseException::Create("Failed to create command pool!");
	}

	void VulkanContext::CreateCommandBuffers()
	{
		m_cmdBuffers.resize(MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocateInfo{};
		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.commandPool = m_cmdPool;
		allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocateInfo.commandBufferCount = static_cast<uint32_t>(m_cmdBuffers.size());
		if (vkAllocateCommandBuffers(m_device, &allocateInfo, m_cmdBuffers.data()) != VK_SUCCESS)
			throw BaseException::Create("Failed to allocate command buffer!");
	}

	void VulkanContext::CreateSyncObjects()
	{
		m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		VkSemaphoreCreateInfo semaphoreCreateInfo{};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			if (vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr,
				&m_imageAvailableSemaphores[i]) != VK_SUCCESS)
			{
				throw BaseException::Create("Failed to create image available semaphore!");
			}
			if (vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr,
				&m_renderFinishedSemaphores[i]) != VK_SUCCESS)
			{
				throw BaseException::Create("Failed to create render finished semaphore!");
			}
			if (vkCreateFence(m_device, &fenceCreateInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS)
			{
				throw BaseException::Create("Failed to create inFlight fence!");
			}
		}
	}
}
