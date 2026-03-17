/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Editor/OpenGLImGuiContext.h"
#include "Exception/Windows/Win32Exception.h"
#include "glad/wgl.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl3.h"

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

	struct WGLWindowData
	{
		HDC hdc;
		HGLRC hglrc;
	};

	OpenGLImGuiContext::OpenGLImGuiContext(const Rendering::OpenGLContext& context)
	{
		HWND hwnd = context.VideoPort().Handle();
    	// 初始化IMGUI
    	IMGUI_CHECKVERSION();
    	ImGui::CreateContext();

    	ImGuiIO& io = ImGui::GetIO();
    	ImFontConfig fontConfig;
    	fontConfig.FontDataOwnedByAtlas = true;
		io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/NotoSans/NotoSansSC-Regular.ttf", 16.0f, &fontConfig);

    	ImGui_ImplWin32_InitForOpenGL(hwnd);
    	ImGui_ImplOpenGL3_Init();

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

    	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    	// ImGui::StyleColorsLight();
		ImGui::StyleColorsDark();
    	ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;

    	ImGuiPlatformIO& platformIO = ImGui::GetPlatformIO();
		assert(platformIO.Renderer_CreateWindow == nullptr);
    	assert(platformIO.Renderer_DestroyWindow == nullptr);
    	assert(platformIO.Renderer_SwapBuffers == nullptr);
    	assert(platformIO.Platform_RenderWindow == nullptr);
    	platformIO.Renderer_CreateWindow = [](ImGuiViewport* viewport) {
    		assert(viewport->RendererUserData == nullptr);

    		auto wglData = TG_NEW WGLWindowData;
    		wglData->hdc = GetDC(static_cast<HWND>(viewport->PlatformHandle));

    	    // 这里的pixelAttribs要与OpenGLContext类构造函数里的pixelAttribs一致
    	    // 可能是因为需要共用HGLRC
    		int pixelAttribs[] =
    		{
    			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
				WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
				WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
				WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
				WGL_COLOR_BITS_ARB,     32,
				WGL_DEPTH_BITS_ARB,     24,
				WGL_STENCIL_BITS_ARB,   8,
				WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,
				0
			};
    		int pixelFormat;
    		UINT numFormats;
    		wglChoosePixelFormatARB(
				wglData->hdc,
				pixelAttribs,
				nullptr,
				1,
				&pixelFormat,
				&numFormats);
    		if (!SetPixelFormat(wglData->hdc, pixelFormat, nullptr))
    			throw Win32Exception::Create("Failed to set the pixel format");

    		wglData->hglrc = wglGetCurrentContext();

    		viewport->RendererUserData = wglData;
    	};
    	platformIO.Renderer_DestroyWindow = [](ImGuiViewport* viewport) {
    		if (auto* data = static_cast<WGLWindowData*>(viewport->RendererUserData))
    		{
    			wglMakeCurrent(nullptr, nullptr);
    			ReleaseDC(static_cast<HWND>(viewport->PlatformHandle), data->hdc);
    			delete data;
    			viewport->RendererUserData = nullptr;
    		}
    	};
    	platformIO.Renderer_SwapBuffers = [](ImGuiViewport* viewport, void*) {
    		if (auto* data = static_cast<WGLWindowData*>(viewport->RendererUserData))
    			SwapBuffers(data->hdc);
    	};
    	platformIO.Platform_RenderWindow = [](ImGuiViewport* viewport, void*) {
    		if (auto* data = static_cast<WGLWindowData*>(viewport->RendererUserData))
    			wglMakeCurrent(data->hdc, data->hglrc);
    	};
    }

	OpenGLImGuiContext::~OpenGLImGuiContext()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void OpenGLImGuiContext::NewFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		// ImGui_ImplWin32_NewFrame->ImGui_ImplWin32_UpdateMouseData->::WindowFromPoint，
		// WindowFromPoint会触发WM_NCHITTEST消息，所以主窗口每帧都会收到WM_NCHITTEST消息
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void OpenGLImGuiContext::Render()
	{
		glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, "Draw Editor UI");

		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();

		glPopDebugGroup();
	}
}
