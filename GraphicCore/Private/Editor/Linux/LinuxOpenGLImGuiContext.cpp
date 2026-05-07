/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Editor/Linux/LinuxOpenGLImGuiContext.h"
#include "imgui_impl_opengl3.h"

namespace TG::Editor
{
    LinuxOpenGLImGuiContext::LinuxOpenGLImGuiContext()
    {
        // 初始化ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        ImFontConfig fontConfig;
        fontConfig.FontDataOwnedByAtlas = true;
        io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/NotoSans/NotoSansSC-Regular.ttf", 16.0f, &fontConfig);

        ImGui_ImplOpenGL3_Init();

        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    LinuxOpenGLImGuiContext::~LinuxOpenGLImGuiContext()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();
    }

    void LinuxOpenGLImGuiContext::NewFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
    }

    void LinuxOpenGLImGuiContext::Render()
    {
        glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, "Draw Editor UI");

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // ImGui::UpdatePlatformWindows();
        // ImGui::RenderPlatformWindowsDefault();

        glPopDebugGroup();
    }
}
