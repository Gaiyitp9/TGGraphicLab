/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "OpenGLExampleFactory.h"
#include "TriangleExample.h"
#include "CubeExample.h"
#include "BasicLightExample.h"

namespace TG
{
    OpenGLExampleFactory::OpenGLExampleFactory(const IDefaultVideoPort& videoPort,
        const ITimer& timer) : m_videoPort(videoPort), m_timer(timer)
    {}

    std::unique_ptr<Example> OpenGLExampleFactory::CreateExample(ExampleEnum example)
    {
        switch (example)
        {
            case ExampleEnum::Triangle:
                return std::make_unique<TriangleExample>();
            case ExampleEnum::Cube:
                return std::make_unique<CubeExample>(m_videoPort, m_timer);
            case ExampleEnum::BasicLight:
                return std::make_unique<BasicLightExample>(m_videoPort, m_timer);
            default:
                return nullptr;
        }
    }
}
