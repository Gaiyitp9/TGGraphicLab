/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Modules/EditorModule.h"
#include "Exception/Windows/Win32Exception.h"
#include "glad/wgl.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl3.h"
#include "Rendering/Texture.h"
#include "Rendering/Color/StandardColors.h"
// #include "imgui_impl_vulkan.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace TG
{
    EditorModule::EditorModule() = default;

    EditorModule::~EditorModule()
    {
    	if (m_isInitialized)
    	{
    		ImGui_ImplOpenGL3_Shutdown();
    		ImGui_ImplWin32_Shutdown();
    		ImGui::DestroyContext();
    	}
    }

	void EditorModule::SetRenderer(Rendering::Renderer* renderer)
	{
		m_renderer = renderer;
	}

    void EditorModule::Update()
    {
    	if (!m_renderer)
    		return;

    	m_renderer->RenderToScreen();

    	Rendering::Color clearColor = Rendering::Black;
    	glClearColor(clearColor.R(), clearColor.G(), clearColor.B(), clearColor.A());
    	glClearDepthf(1.0f);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  		ImGui_ImplOpenGL3_NewFrame();
  		// ImGui_ImplWin32_NewFrame->ImGui_ImplWin32_UpdateMouseData->::WindowFromPoint，
  		// WindowFromPoint会触发WM_NCHITTEST消息，所以主窗口每帧都会收到WM_NCHITTEST消息
  		ImGui_ImplWin32_NewFrame();
  		ImGui::NewFrame();

  		ImGuiWindowFlags windowFlags =
  			ImGuiWindowFlags_MenuBar |
  			ImGuiWindowFlags_NoDocking |
  			ImGuiWindowFlags_NoTitleBar |
  			ImGuiWindowFlags_NoCollapse |
  			ImGuiWindowFlags_NoResize |
  			ImGuiWindowFlags_NoMove|
  			ImGuiWindowFlags_NoBringToFrontOnFocus |
  			ImGuiWindowFlags_NoNavFocus |
  			ImGuiWindowFlags_NoBackground;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    	ImGui::Begin("Main DockSpace", nullptr, windowFlags);
		ImGui::PopStyleVar(3);

		// Submit the DockSpace
		ImGuiID dockSpaceId = ImGui::GetID("MyDockSpace");
    	static ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;
		ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), dockSpaceFlags);

		if (ImGui::BeginMenuBar())
		{
		  if (ImGui::BeginMenu("Options"))
		  {
		      if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockSpaceFlags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) { dockSpaceFlags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode; }
		      if (ImGui::MenuItem("Flag: NoDockingSplit",         "", (dockSpaceFlags & ImGuiDockNodeFlags_NoDockingSplit) != 0))             { dockSpaceFlags ^= ImGuiDockNodeFlags_NoDockingSplit; }
		      if (ImGui::MenuItem("Flag: NoUndocking",            "", (dockSpaceFlags & ImGuiDockNodeFlags_NoUndocking) != 0))                { dockSpaceFlags ^= ImGuiDockNodeFlags_NoUndocking; }
		      if (ImGui::MenuItem("Flag: NoResize",               "", (dockSpaceFlags & ImGuiDockNodeFlags_NoResize) != 0))                   { dockSpaceFlags ^= ImGuiDockNodeFlags_NoResize; }
		      if (ImGui::MenuItem("Flag: AutoHideTabBar",         "", (dockSpaceFlags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))             { dockSpaceFlags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
		      ImGui::Separator();

		      ImGui::EndMenu();
		  }
		  ImGui::EndMenuBar();
		}

		ImGui::End();

    	ImGui::Begin("Scene");
    	ImVec2 availRegion = ImGui::GetContentRegionAvail();
    	auto targetWidth = static_cast<unsigned int>(availRegion.x);
    	auto targetHeight = static_cast<unsigned int>(availRegion.y);
    	if (targetWidth != m_sceneWidth || targetHeight != m_sceneHeight)
    	{
    		onSceneViewportResize.Broadcast(targetWidth, targetHeight);
    		m_sceneWidth = targetWidth;
    		m_sceneHeight = targetHeight;
    	}
    	ImGui::Image(
    		Rendering::CastID<Rendering::OpenGLID>(m_renderer->RenderTarget()->GetID()),
    		availRegion,
    		ImVec2(0.0f, 1.0f),
    		ImVec2(1.0f, 0.0f)
    	);
    	ImGui::End();

    	onDrawUI.Broadcast();
    }

	void EditorModule::PostUpdate()
    {
    	ImGui::Render();

    	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    	ImGui::UpdatePlatformWindows();
    	ImGui::RenderPlatformWindowsDefault();
    }

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

    void EditorModule::PlugInVideoPort(const IDefaultVideoPort& display)
    {
    	// 初始化IMGUI
    	IMGUI_CHECKVERSION();
    	ImGui::CreateContext();
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

    	ImGuiIO& io = ImGui::GetIO();
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
}
