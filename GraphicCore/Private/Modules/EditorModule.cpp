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
// #include "imgui_impl_vulkan.h"

namespace TG
{
    EditorModule::EditorModule() = default;

    EditorModule::~EditorModule() = default;

	void EditorModule::SetRenderer(Rendering::Renderer* renderer)
	{
		m_renderer = renderer;
    	m_editorContext = std::make_unique<Editor::EditorContext>(m_renderer->VideoPort());
	}

    void EditorModule::Update()
    {
    	if (!m_renderer)
    		return;

    	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, "Draw Editor UI");

    	m_renderer->RenderToScreen();

    	glClearColor(0.45f, 0.56f, 0.60f, 1.0f);
    	glClearDepthf(1.0f);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  		ImGui_ImplOpenGL3_NewFrame();
  		// ImGui_ImplWin32_NewFrame->ImGui_ImplWin32_UpdateMouseData->::WindowFromPoint，
  		// WindowFromPoint会触发WM_NCHITTEST消息，所以主窗口每帧都会收到WM_NCHITTEST消息
  		ImGui_ImplWin32_NewFrame();
  		ImGui::NewFrame();

  		ImGuiWindowFlags windowFlags =
  			// ImGuiWindowFlags_MenuBar |
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

		// if (ImGui::BeginMenuBar())
		// {
		//   if (ImGui::BeginMenu("Options"))
		//   {
		//       if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockSpaceFlags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) { dockSpaceFlags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode; }
		//       if (ImGui::MenuItem("Flag: NoDockingSplit",         "", (dockSpaceFlags & ImGuiDockNodeFlags_NoDockingSplit) != 0))             { dockSpaceFlags ^= ImGuiDockNodeFlags_NoDockingSplit; }
		//       if (ImGui::MenuItem("Flag: NoUndocking",            "", (dockSpaceFlags & ImGuiDockNodeFlags_NoUndocking) != 0))                { dockSpaceFlags ^= ImGuiDockNodeFlags_NoUndocking; }
		//       if (ImGui::MenuItem("Flag: NoResize",               "", (dockSpaceFlags & ImGuiDockNodeFlags_NoResize) != 0))                   { dockSpaceFlags ^= ImGuiDockNodeFlags_NoResize; }
		//       if (ImGui::MenuItem("Flag: AutoHideTabBar",         "", (dockSpaceFlags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))             { dockSpaceFlags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
		//       ImGui::Separator();
		//
		//       ImGui::EndMenu();
		//   }
		//   ImGui::EndMenuBar();
		// }

		ImGui::End();

    	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
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
    	ImGui::PopStyleVar(1);

    	ImGui::Begin("Log");
    	ImGui::TextUnformatted("Test");
    	ImGui::End();

    	onDrawUI.Broadcast();

    	glPopDebugGroup();
    }

	void EditorModule::PostUpdate()
    {
    	ImGui::Render();

    	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    	ImGui::UpdatePlatformWindows();
    	ImGui::RenderPlatformWindowsDefault();
    }
}
