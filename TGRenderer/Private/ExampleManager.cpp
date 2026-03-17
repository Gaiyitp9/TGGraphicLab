/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "ExampleManager.h"
#include "OpenGLExample/BasicLightExample.h"
#include "OpenGLExample/CubeExample.h"
#include "OpenGLExample/TriangleExample.h"
#include "VulkanExample/TriangleVkExample.h"

namespace TG
{
    ExampleManager::ExampleManager(Rendering::Renderer* renderer, const ITimer& timer, ExampleEnum example)
        : m_renderer(renderer), m_timer(timer), m_currentExample(example)
    {
        SetCurrentExample(m_currentExample);
    }

    ExampleManager::~ExampleManager()
    {

    }

    void ExampleManager::SetCurrentExample(ExampleEnum example)
    {
        m_currentExample = example;
        if (m_renderer->API() ==  Rendering::GraphicsAPI::OpenGL)
        {
            switch (example)
            {
                case ExampleEnum::Triangle:
                    m_example = std::make_unique<TriangleExample>();
                    break;
                case ExampleEnum::Cube:
                    m_example = std::make_unique<CubeExample>(m_renderer, m_timer);
                    break;
                case ExampleEnum::BasicLight:
                    m_example = std::make_unique<BasicLightExample>(m_renderer, m_timer);
                    break;
                default:
                    m_example = std::make_unique<BasicLightExample>(m_renderer, m_timer);
            }
        }
        else if (m_renderer->API() ==  Rendering::GraphicsAPI::Vulkan)
        {
            switch (example)
            {
                case ExampleEnum::Triangle:
                    m_example = std::make_unique<TriangleVkExample>(m_renderer);
                    break;
                default:
                    m_example = std::make_unique<TriangleVkExample>(m_renderer);
            }
        }
    }

    void ExampleManager::Subscribe(MulticastDelegate<void()>& onDraw,
        MulticastDelegate<void()>& onDrawUI,
        MulticastDelegate<void(int, int, unsigned, unsigned)>& sceneViewportChangedDelegate)
    {
        onDraw.Add([&example = m_example]{ example->Draw(); });
        onDrawUI.Add([&example = m_example]{ example->DrawUI(); });
        sceneViewportChangedDelegate.Add(
            [&example = m_example](int, int, unsigned width, unsigned height){
                example->OnViewportChanged(width, height);
            }
        );
    }
}
