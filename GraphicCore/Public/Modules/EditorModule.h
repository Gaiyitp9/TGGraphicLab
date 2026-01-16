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

        void SetRenderer(Renderer* renderer);
        void PlugInVideoPort(const IDefaultVideoPort& display);

        void Update() override;
        void PostUpdate() override;

    private:
        Renderer* m_renderer{ nullptr };
        bool m_isInitialized{ false };
        // bool m_showDemoWindow{ true };
        // bool m_showAnotherWindow{ false };
    };
}
