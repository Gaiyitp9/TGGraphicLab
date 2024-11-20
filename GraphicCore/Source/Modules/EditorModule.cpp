/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Modules/EditorModule.h"
#include "Exception/EGLException.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl3.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace TG
{
    EditorModule::EditorModule() = default;

    EditorModule::~EditorModule()
    {
    	ImGui_ImplOpenGL3_Shutdown();
    	ImGui_ImplWin32_Shutdown();
    	ImGui::DestroyContext();
    }

	void EditorModule::PreUpdate()
	{

	}

    void EditorModule::Update()
    {
    	float clearColor[4]{ 0.2f, 0.3f, 0.3f, 1.0f };

    	ImGuiIO& io = ImGui::GetIO();

    	ImGui_ImplOpenGL3_NewFrame();
    	ImGui_ImplWin32_NewFrame();
    	ImGui::NewFrame();

    	if (m_showDemoWindow)
    		ImGui::ShowDemoWindow(&m_showDemoWindow);

	    {
    		static float f = 0.0f;
    		static int counter = 0;

    		ImGui::Begin("Hello World!");

    		ImGui::Text("This is some useful text.");
    		ImGui::Checkbox("Demo Window", &m_showDemoWindow);
    		ImGui::Checkbox("Another Window", &m_showAnotherWindow);

    		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    		ImGui::ColorEdit3("Color", clearColor);

    		if (ImGui::Button("Button"))
    			++counter;
    		ImGui::SameLine();
    		ImGui::Text("Counter = %d", counter);

    		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    		ImGui::End();
	    }

    	if (m_showAnotherWindow)
    	{
    		ImGui::Begin("Another Window", &m_showAnotherWindow);
    		ImGui::Text("Hello from another window!");
    		if (ImGui::Button("Close Me"))
    			m_showAnotherWindow = false;
    		ImGui::End();
    	}

    	ImGui::Render();

    	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    	ImGui::UpdatePlatformWindows();
    	ImGui::RenderPlatformWindowsDefault();
    }

	void EditorModule::PostUpdate()
    {

    }

	using Win32Proc = LRESULT (WINAPI*)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static Win32Proc g_prevWndProc = nullptr;
	static LRESULT ImGuiWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		assert(g_prevWndProc != nullptr);

		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
			return true;

		return g_prevWndProc(hwnd, msg, wParam, lParam);
	}

	struct EGLData
	{
		EGLDisplay display;
		EGLContext context;
		EGLSurface surface;
	};

    void EditorModule::PlugInVideoPlay(const IVideoDisplay& display)
    {
        // 窗口程序插入ImGui处理输入事件的代码
    	g_prevWndProc = reinterpret_cast<Win32Proc>(GetWindowLongPtrW(display.GetHandle(), GWLP_WNDPROC));
    	SetWindowLongPtrW(display.GetHandle(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(ImGuiWindowProc));

    	// 初始化IMGUI
    	IMGUI_CHECKVERSION();
    	ImGui::CreateContext();
    	ImGuiIO& io = ImGui::GetIO();
    	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    	ImGui::StyleColorsDark();

    	ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 0.0f;
    	style.Colors[ImGuiCol_WindowBg].w = 1.0f;

    	ImGui_ImplWin32_InitForOpenGL(display.GetHandle());
    	ImGui_ImplOpenGL3_Init();

    	ImGuiPlatformIO& platformIO = ImGui::GetPlatformIO();
		assert(platformIO.Renderer_CreateWindow == nullptr);
    	assert(platformIO.Renderer_DestroyWindow == nullptr);
    	assert(platformIO.Renderer_SwapBuffers == nullptr);
    	assert(platformIO.Platform_RenderWindow == nullptr);
    	platformIO.Renderer_CreateWindow = [](ImGuiViewport* viewport) {
    		assert(viewport->RendererUserData == nullptr);

    		auto* data = TG_NEW EGLData;
    		data->display = eglGetCurrentDisplay();
    		data->context = eglGetCurrentContext();

    		// ChooseEGLConfig
    		const EGLint configurationAttributes[] = {
    			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
				EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
				EGL_NONE
			};
    		EGLint numConfigs;
			EGLConfig eglConfig;
    		if (eglChooseConfig(data->display, configurationAttributes, &eglConfig, 1, &numConfigs) != GL_TRUE)
    			throw EGLException("Failed to choose EGLConfig");
    		if (numConfigs != 1)
    			throw EGLException("eglChooseConfig return no config");

    		// 创建EGLSurface
    		data->surface = eglCreateWindowSurface(data->display, eglConfig,
				static_cast<EGLNativeWindowType>(viewport->PlatformHandle), nullptr);
    		if (data->surface == EGL_NO_SURFACE)
    			throw EGLException("Failed to create EGLSurface");

    		viewport->RendererUserData = data;
    	};
    	platformIO.Renderer_DestroyWindow = [](ImGuiViewport* viewport) {
    		if (viewport->RendererUserData != nullptr)
    		{
    			auto* data = static_cast<EGLData *>(viewport->RendererUserData);
    			if (eglDestroySurface(data->display, data->surface) != EGL_TRUE)
    			{
    				delete data;
    				throw EGLException("Failed to destroy EGLSurface");
    			}
    			delete data;
    			viewport->RendererUserData = nullptr;
    		}
    	};
    	platformIO.Renderer_SwapBuffers = [](ImGuiViewport* viewport, void*) {
    		if (auto* data = static_cast<EGLData*>(viewport->RendererUserData))
    		{
    			if (eglSwapBuffers(data->display, data->surface) != EGL_TRUE)
    				throw EGLException("Failed to swap buffers");
    		}
    	};
    	platformIO.Platform_RenderWindow = [](ImGuiViewport* viewport, void*) {
    		if (auto* data = static_cast<EGLData*>(viewport->RendererUserData))
    		{
    			if (eglMakeCurrent(data->display, data->surface, data->surface, data->context) != EGL_TRUE)
    				throw EGLException("Failed to make current surface");
    		}
    	};
    }
}
