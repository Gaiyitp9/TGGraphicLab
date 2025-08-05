/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/Renderer.h"
#include "Base/CommonInterfaces.h"
#include "Rendering/OpenGL/OpenGLContext.h"
#include "Rendering/Example.h"
#include <memory>

namespace TG::Rendering
{
    class OpenGLRenderer final : public Renderer
    {
    public:
        OpenGLRenderer(const std::weak_ptr<IDefaultVideoPort>& videoPort, const std::weak_ptr<ITimer>& timer);
        ~OpenGLRenderer() override;

        void Render() override;
        void Present() override;
        void FrameBufferResizeCallback(unsigned int width, unsigned int height) override;

    private:
        OpenGLContext m_context;

        std::unique_ptr<ExampleFactory> m_exampleFactory;
        std::unique_ptr<Example> m_example;
        ExampleEnum m_exampleEnum = ExampleEnum::BasicLight;
    };
}
