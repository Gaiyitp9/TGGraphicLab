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

        void SetupRenderer(Rendering::GraphicsAPI api, const IDefaultVideoPort& videoPort);
        void Subscribe(MulticastDelegate<void(unsigned, unsigned)>& windowResizeDelegate,
            MulticastDelegate<void(int, int, unsigned, unsigned)>& sceneViewportChangedDelegate);

        [[nodiscard]] Rendering::Renderer* GetRenderer() const noexcept { return m_Renderer.get(); }

        void Update() override;
        void PostUpdate() override;

        // 绘制委托
        MulticastDelegate<void()> onDraw;

    private:
        Rendering::GraphicsAPI m_GraphicsAPI{ Rendering::GraphicsAPI::OpenGL };
        std::unique_ptr<Rendering::Renderer> m_Renderer;
    };
}
