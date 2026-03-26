/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Modules/EditorModule.h"
#include "Editor/OpenGLImGuiContext.h"
#include "Editor/VulkanImGuiContext.h"
#include "Rendering/OpenGL/OpenGLRenderer.h"
#include "Rendering/Vulkan/VulkanRenderer.h"
#include "imgui.h"
#include "Rendering/Texture.h"

namespace TG
{
    EditorModule::EditorModule() = default;

    EditorModule::~EditorModule() = default;

	void EditorModule::SetRenderer(Rendering::Renderer* renderer)
	{
		m_renderer = renderer;
		if (m_renderer->API() == Rendering::GraphicsAPI::OpenGL)
		{
    		m_editorContext = std::make_unique<Editor::OpenGLImGuiContext>(
    			dynamic_cast<Rendering::OpenGLRenderer*>(m_renderer)->GetContext()
    		);
		}
		else if (m_renderer->API() == Rendering::GraphicsAPI::Vulkan)
		{
			m_editorContext = std::make_unique<Editor::VulkanImGuiContext>(
				dynamic_cast<Rendering::VulkanRenderer*>(m_renderer)->GetContext()
			);
		}
	}

    void EditorModule::Update()
    {
    	if (!m_renderer)
    		return;

		m_editorContext->NewFrame();

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
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0,0,0,0));
    	static ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;
		ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), dockSpaceFlags);
		ImGui::PopStyleColor();

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
    	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoBackground);
		ImVec2 cursorScreenPos  = ImGui::GetCursorScreenPos();
		ImVec2 windowPos = ImGui::GetMainViewport()->Pos;
		ImVec2 size = ImGui::GetContentRegionAvail();
		auto targetPosX = static_cast<int>(cursorScreenPos.x - windowPos.x);
		auto targetPosY = static_cast<int>(cursorScreenPos.y - windowPos.y);
    	auto targetWidth = static_cast<unsigned int>(size.x);
    	auto targetHeight = static_cast<unsigned int>(size.y);
    	if (targetPosX != m_scenePosX || targetPosY != m_scenePosY ||
    		targetWidth != m_sceneWidth || targetHeight != m_sceneHeight)
    	{
    		onSceneViewportResize.Broadcast(targetPosX, targetPosY, targetWidth, targetHeight);
    		m_scenePosX = targetPosX;
    		m_scenePosY = targetPosY;
    		m_sceneWidth = targetWidth;
    		m_sceneHeight = targetHeight;
    	}
    	ImGui::End();
    	ImGui::PopStyleVar();

    	ImGui::Begin("Log");
    	ImGui::TextUnformatted("Test");
    	ImGui::End();

    	onDrawUI.Broadcast();

		m_renderer->RenderToUI();

		m_editorContext->Render();
    }

	void EditorModule::PostUpdate()
    {

    }
}
