/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Module.h"
#include "Base/CommonInterfaces.h"
#include "Rendering/Renderer.h"
#include "Editor/ImGuiContext.h"

namespace TG
{
    class EditorModule final : public Module
    {
    public:
        EditorModule();
        EditorModule(const EditorModule&) = delete;
        EditorModule(EditorModule&&) = delete;
        EditorModule& operator=(const EditorModule&) = delete;
        EditorModule& operator=(EditorModule&&) = delete;
        ~EditorModule() override;

        void SetRenderer(Rendering::Renderer* renderer);

        void Update() override;
        void PostUpdate() override;

        // Scene窗口变化委托
        MulticastDelegate<void(int, int, unsigned, unsigned)> onSceneViewportResize;

        // 绘制委托
        MulticastDelegate<void()> onDrawUI;

    private:
        Rendering::Renderer* m_renderer{ nullptr };
    	std::unique_ptr<Editor::ImGuiContext> m_editorContext;

        int m_scenePosX{ 0 };
        int m_scenePosY{ 0 };
        unsigned int m_sceneWidth{ 0 };
        unsigned int m_sceneHeight{ 0 };
    };
}
