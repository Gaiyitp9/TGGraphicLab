/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Modules/EditorModule.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl3.h"

namespace TG
{
    EditorModule::EditorModule()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui::StyleColorsDark();

        // ImGui_ImplWin32_Init(m_platformModule.GetWindow().GetWindowHandle());
        ImGui_ImplOpenGL3_Init();
    }

    EditorModule::~EditorModule()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    void EditorModule::Update()
    {

    }
}
