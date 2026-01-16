/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Module.h"
#include "Rendering/Renderer.h"
#include "Base/CommonInterfaces.h"
#include "Rendering/GraphicsAPIDefines.h"
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

        void SetupRenderer(const IDefaultVideoPort& videoPort, const ITimer& timer);
        void Subscribe(MulticastDelegate<void(unsigned, unsigned)>& windowResizeDelegate);

        [[nodiscard]] Renderer* GetRenderer() const noexcept { return m_Renderer.get(); }

        void Update() override;
        void PostUpdate() override;

    private:
        Rendering::GraphicsAPI m_GraphicsAPI{ Rendering::GraphicsAPI::OpenGL };
        std::unique_ptr<Renderer> m_Renderer;
    };
}
