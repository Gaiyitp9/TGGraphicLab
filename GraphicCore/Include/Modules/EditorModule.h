/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Module.h"
#include "Base/Window.h"

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

        void PreUpdate() override;
        void Update() override;
        void PostUpdate() override;

        void PlugInVideoPlay(const IVideoDisplay& display);

    private:
        bool m_showDemoWindow{ true };
        bool m_showAnotherWindow{ false };
    };
}
