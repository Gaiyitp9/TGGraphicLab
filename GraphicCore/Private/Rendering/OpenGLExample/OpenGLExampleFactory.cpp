/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "OpenGLExampleFactory.h"
#include "Rendering/OpenGLExample/QuadExample.h"
#include "Rendering/OpenGLExample/CubeExample.h"
#include "Rendering/OpenGLExample/LightExample.h"

namespace TG::Rendering
{
    OpenGlExampleFactory::OpenGlExampleFactory(const std::weak_ptr<IDefaultVideoPort>& videoPort,
        const std::weak_ptr<ITimer>& timer) : m_videoPort(videoPort), m_timer(timer)
    {}

    std::unique_ptr<Example> OpenGlExampleFactory::CreateExample(ExampleEnum example)
    {
        switch (example)
        {
            case ExampleEnum::Quad:
                return std::make_unique<QuadExample>(m_videoPort, m_timer);
            case ExampleEnum::Cube:
                return std::make_unique<CubeExample>(m_videoPort, m_timer);
            case ExampleEnum::BasicLight:
                return std::make_unique<LightExample>(m_videoPort, m_timer);
            default:
                return nullptr;
        }
    }
}
