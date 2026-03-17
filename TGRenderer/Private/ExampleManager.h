/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Example.h"
#include "Rendering/Renderer.h"

namespace TG
{
    class ExampleManager
    {
    public:
        ExampleManager(Rendering::Renderer* renderer, const ITimer& timer, ExampleEnum example);
        ~ExampleManager();

        void SetCurrentExample(ExampleEnum example);

        void Subscribe(MulticastDelegate<void()>& onDraw,
            MulticastDelegate<void()>& onDrawUI,
            MulticastDelegate<void(int, int, unsigned, unsigned)>& sceneViewportChangedDelegate);

    private:
        Rendering::Renderer* m_renderer;
        const ITimer& m_timer;
        ExampleEnum m_currentExample{ ExampleEnum::Triangle };
        std::unique_ptr<Example> m_example;
    };
}
