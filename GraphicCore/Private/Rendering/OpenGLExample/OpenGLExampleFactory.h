/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/Example.h"
#include "Base/CommonInterfaces.h"

namespace TG::Rendering
{
    class OpenGlExampleFactory : public ExampleFactory
    {
    public:
        OpenGlExampleFactory(const std::weak_ptr<IDefaultVideoPort>& videoPort, const std::weak_ptr<ITimer>& timer);
        ~OpenGlExampleFactory() override = default;

        std::unique_ptr<Example> CreateExample(ExampleEnum example) override;

    private:
        std::weak_ptr<IDefaultVideoPort> m_videoPort;
        std::weak_ptr<ITimer> m_timer;
    };
}
