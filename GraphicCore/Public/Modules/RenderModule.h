/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Module.h"
#include "Base/Window.h"
#include "Rendering/Renderer.h"
#include <memory>

namespace TG
{
    class RenderModule final : public Module
    {
    public:
        RenderModule();
        RenderModule(const RenderModule&) = delete;
        RenderModule(RenderModule&&) = delete;
        RenderModule& operator=(const RenderModule&) = delete;
        RenderModule& operator=(RenderModule&&) = delete;
        ~RenderModule() override;

        void PlugInVideoPort(const IVideoPort& videoPort);
        [[nodiscard]] const std::shared_ptr<Renderer>& GetRenderer() const noexcept { return m_Renderer; }

        void Update() override;
        void PostUpdate() override;

    private:
        std::shared_ptr<Renderer> m_Renderer;
    };
}
