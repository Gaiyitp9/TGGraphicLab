/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Example.h"
#include "Base/CommonInterfaces.h"

namespace TG
{
    class VulkanExampleFactory : public ExampleFactory
    {
    public:
        VulkanExampleFactory(const IDefaultVideoPort& videoPort, const ITimer& timer);
        ~VulkanExampleFactory() override = default;

        std::unique_ptr<Example> CreateExample(ExampleEnum example) override;

    private:
        const IDefaultVideoPort& m_videoPort;
        const ITimer& m_timer;
    };
}
