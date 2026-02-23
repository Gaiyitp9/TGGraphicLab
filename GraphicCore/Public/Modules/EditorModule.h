/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Module.h"
#include "Base/CommonInterfaces.h"
#include "Rendering/Renderer.h"

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
        void PlugInVideoPort(const IDefaultVideoPort& display);

        void Update() override;
        void PostUpdate() override;

        // Scene窗口尺寸变化委托
        MulticastDelegate<void(unsigned, unsigned)> onSceneViewportResize;

        // 绘制委托
        MulticastDelegate<void()> onDrawUI;

    private:
        Rendering::Renderer* m_renderer{ nullptr };
        bool m_isInitialized{ false };
        unsigned int m_sceneWidth{ 0 };
        unsigned int m_sceneHeight{ 0 };
    };
}
