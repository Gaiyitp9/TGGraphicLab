/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "VulkanExampleFactory.h"
#include "TriangleVkExample.h"

namespace TG
{
    VulkanExampleFactory::VulkanExampleFactory(const IDefaultVideoPort& videoPort,
        const ITimer& timer) : m_videoPort(videoPort), m_timer(timer)
    {}

    std::unique_ptr<Example> VulkanExampleFactory::CreateExample(ExampleEnum example)
    {
        switch (example)
        {
            case ExampleEnum::Triangle:
                return std::make_unique<TriangleVkExample>(m_videoPort);
            default:
                return nullptr;
        }
    }
}
