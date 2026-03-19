/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Editor/VulkanImGuiContext.h"
#include "Exception/Windows/Win32Exception.h"
#include "imgui_impl_vulkan.h"
#include "imgui_impl_win32.h"
#include "vulkan/vulkan_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace TG::Editor
{
	using Win32Proc = LRESULT (WINAPI*)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static Win32Proc g_prevWndProc = nullptr;
	static LRESULT ImGuiWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		assert(g_prevWndProc != nullptr);

		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
			return 1;
		return g_prevWndProc(hwnd, msg, wParam, lParam);
	}

	static int ImGui_ImplWin32_CreateVkSurface(ImGuiViewport* viewport, ImU64 vk_instance,
		const void* vk_allocator, ImU64* out_vk_surface)
	{
		VkWin32SurfaceCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfo.hwnd = static_cast<HWND>(viewport->PlatformHandleRaw);
		createInfo.hinstance = GetModuleHandle(nullptr);
		return vkCreateWin32SurfaceKHR(
			reinterpret_cast<VkInstance>(vk_instance),
			&createInfo,
			static_cast<const VkAllocationCallbacks*>(vk_allocator),
			reinterpret_cast<VkSurfaceKHR*>(out_vk_surface)
		);
	}

	VulkanImGuiContext::VulkanImGuiContext(const Rendering::VulkanContext& context)
		: m_context(context)
	{
		HWND hwnd = context.VideoPort().Handle();

		// 窗口程序插入ImGui处理输入事件的代码
		g_prevWndProc = reinterpret_cast<Win32Proc>(GetWindowLongPtrW(hwnd, GWLP_WNDPROC));
		if (!g_prevWndProc)
			throw Win32Exception::Create("Failed to get window procedure");
		SetLastError(0);
		if (SetWindowLongPtrW(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(ImGuiWindowProc)) == 0
			&& GetLastError() != 0)
		{
			throw Win32Exception::Create("Failed to set window procedure");
		}

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		ImFontConfig fontConfig;
		fontConfig.FontDataOwnedByAtlas = true;
		io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/NotoSans/NotoSansSC-Regular.ttf",
			16.0f, &fontConfig);

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		// ImGui::StyleColorsLight();

		ImGuiStyle& style = ImGui::GetStyle();
		 // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;

		// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(hwnd);
		ImGui::GetPlatformIO().Platform_CreateVkSurface = ImGui_ImplWin32_CreateVkSurface;

		VkFormat colorAttachmentFormat = VK_FORMAT_R8G8B8A8_SRGB;
		VkPipelineRenderingCreateInfoKHR pipelineRenderingInfo{};
		pipelineRenderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
		pipelineRenderingInfo.colorAttachmentCount = 1;
		pipelineRenderingInfo.pColorAttachmentFormats = &colorAttachmentFormat;

		ImGui_ImplVulkan_InitInfo initInfo{};
		initInfo.ApiVersion = VK_API_VERSION_1_4;
		initInfo.Instance = m_context.GetInstance();
		initInfo.PhysicalDevice = m_context.GetPhysicalDevice();
		initInfo.Device = m_context.GetDevice();
		initInfo.QueueFamily = m_context.GetQueueFamily(Rendering::VkQueueType::Graphic);
		initInfo.Queue = m_context.GetQueue(Rendering::VkQueueType::Graphic);
		initInfo.PipelineCache = VK_NULL_HANDLE;
		initInfo.DescriptorPoolSize = IMGUI_IMPL_VULKAN_MINIMUM_IMAGE_SAMPLER_POOL_SIZE;
		initInfo.MinImageCount = m_context.GetSwapChainMinImageCount();
		initInfo.ImageCount = static_cast<uint32_t>(m_context.GetSwapChainImageViews().size());
		initInfo.Allocator = VK_NULL_HANDLE;
		initInfo.UseDynamicRendering = true;
		initInfo.PipelineInfoMain.Subpass = 0;
		initInfo.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		initInfo.PipelineInfoMain.PipelineRenderingCreateInfo = pipelineRenderingInfo;
		ImGui_ImplVulkan_Init(&initInfo);
	}

	VulkanImGuiContext::~VulkanImGuiContext()
	{
		VkDevice device = m_context.GetDevice();

        vkDeviceWaitIdle(device);

		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void VulkanImGuiContext::NewFrame()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void VulkanImGuiContext::Render()
	{
		ImGui::Render();

		VkCommandBuffer cmdBuffer = m_context.GetCommandBuffer();

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = m_context.GetSwapChainImages()[m_context.GetCurrentImageIndex()];
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.layerCount = 1;
		vkCmdPipelineBarrier(
			cmdBuffer,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		VkRenderingAttachmentInfo attachmentInfo{};
		attachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
		attachmentInfo.imageView = m_context.GetSwapChainImageViews()[m_context.GetCurrentImageIndex()];
		attachmentInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		attachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
		attachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachmentInfo.clearValue = { 0.45f, 0.56f, 0.60f, 1.0f };

		VkRenderingInfo renderingInfo{};
		renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
		renderingInfo.renderArea.extent = m_context.GetSwapChainExtent();
		renderingInfo.layerCount = 1;
		renderingInfo.colorAttachmentCount = 1;
		renderingInfo.pColorAttachments = &attachmentInfo;

		vkCmdBeginRendering(cmdBuffer, &renderingInfo);

		// Record dear imgui primitives into command buffer
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmdBuffer);

		vkCmdEndRendering(cmdBuffer);

		VkImageMemoryBarrier barrier2{};
		barrier2.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier2.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		barrier2.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		barrier2.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		barrier2.dstAccessMask = 0;
		barrier2.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier2.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier2.image = m_context.GetSwapChainImages()[m_context.GetCurrentImageIndex()];
		barrier2.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier2.subresourceRange.levelCount = 1;
		barrier2.subresourceRange.layerCount = 1;
		vkCmdPipelineBarrier(
			cmdBuffer,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier2
		);

		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}
