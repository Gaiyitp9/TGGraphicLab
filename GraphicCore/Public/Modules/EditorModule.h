/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Module.h"
#include "Base/CommonInterfaces.h"
#include "Rendering/Renderer.h"
#include <memory>

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

        void SetRenderer(const std::weak_ptr<Renderer>& renderer);
        void PlugInVideoPort(const std::weak_ptr<IDefaultVideoPort>& display);

        void Update() override;
        void PostUpdate() override;

    private:
        std::weak_ptr<Renderer> m_renderer;
        bool m_isInitialized{ false };
        // bool m_showDemoWindow{ true };
        // bool m_showAnotherWindow{ false };
    };
}
