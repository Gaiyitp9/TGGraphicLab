/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Editor/EditorContext.h"
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

	EditorContext::EditorContext(const IDefaultVideoPort& display)
	{
    	// 初始化IMGUI
    	IMGUI_CHECKVERSION();
    	ImGui::CreateContext();

    	ImGuiIO& io = ImGui::GetIO();
    	ImFontConfig fontConfig;
    	fontConfig.FontDataOwnedByAtlas = true;
		io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/NotoSans/NotoSansSC-Regular.ttf", 16.0f, &fontConfig);

    	ImGui_ImplWin32_InitForOpenGL(display.Handle());
    	ImGui_ImplOpenGL3_Init();

    	m_isInitialized = true;

        // 窗口程序插入ImGui处理输入事件的代码
    	g_prevWndProc = reinterpret_cast<Win32Proc>(GetWindowLongPtrW(display.Handle(), GWLP_WNDPROC));
    	if (!g_prevWndProc)
    		throw Win32Exception::Create("Failed to get window procedure");
    	SetLastError(0);
	    if (SetWindowLongPtrW(display.Handle(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(ImGuiWindowProc)) == 0
	    	&& GetLastError() != 0)
	    {
    		throw Win32Exception::Create("Failed to set window procedure");
	    }

    	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    	ImGui::StyleColorsDark();
    	// ImGuiStyle& style = ImGui::GetStyle();
		// style.WindowRounding = 0.0f;
    	// style.Colors[ImGuiCol_WindowBg].w = 1.0f;

    	ImGuiPlatformIO& platformIO = ImGui::GetPlatformIO();
		assert(platformIO.Renderer_CreateWindow == nullptr);
    	assert(platformIO.Renderer_DestroyWindow == nullptr);
    	assert(platformIO.Renderer_SwapBuffers == nullptr);
    	assert(platformIO.Platform_RenderWindow == nullptr);
    	platformIO.Renderer_CreateWindow = [](ImGuiViewport* viewport) {
    		assert(viewport->RendererUserData == nullptr);

    		auto wglData = TG_NEW WGLWindowData;
    		wglData->hdc = GetDC(static_cast<HWND>(viewport->PlatformHandle));
    	    // 这里的PIXELFORMATDESCRIPTOR要与OpenGLContext类构造函数里的PIXELFORMATDESCRIPTOR一致
    	    // 可能是因为需要共用HGLRC
    	    PIXELFORMATDESCRIPTOR pfd{
    	        sizeof(PIXELFORMATDESCRIPTOR),
                1,
                PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
                PFD_TYPE_RGBA,
                32,
                0, 0, 0, 0, 0, 0, 8, 0,
                0, 0, 0, 0, 0,
                24,
                8,
                0,
                0,
                0, 0, 0, 0
            };
    		int pixelFormat = ChoosePixelFormat(wglData->hdc, &pfd);
    		if (pixelFormat == 0)
    			throw BaseException::Create("Failed to get a valid ChoosePixelFormat");
    		if (!SetPixelFormat(wglData->hdc, pixelFormat, &pfd))
    			throw BaseException::Create("Failed to set the pixel format");
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

	EditorContext::~EditorContext()
	{
		if (m_isInitialized)
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();
		}
	}
}
