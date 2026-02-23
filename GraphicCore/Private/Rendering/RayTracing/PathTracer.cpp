/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Rendering/RayTracing/PathTracer.h"
#include "Diagnostic/Log.hpp"
#include "Rendering/RayTracing/Sphere.h"
#include "Rendering/RayTracing/Plane.h"
#include "Rendering/RayTracing/Material.h"
#include "Math/Random.hpp"

namespace TG::Rendering::RayTracing
{
    PathTracer::PathTracer()
    {
        auto materialGround = std::make_shared<Lambert>(Color(0.5f, 0.5f, 0.5f));
        m_scene.Add(std::make_shared<Plane>(
            Math::Vector3f{ 0.0f, 1.0f, 0.0f },
            std::vector<Math::Vector3f>
            {
                { -1000.0f, 0.0f, -1000.0f },
                {  1000.0f, 0.0f, -1000.0f },
                {  1000.0f, 0.0f,  1000.0f },
                { -1000.0f, 0.0f,  1000.0f }
            },
            materialGround
        ));

        for (int i = -5; i <= 5; ++i)
        {
            for (int j = -5; j <= 5; ++j)
            {
                Math::Vector3f center{
                    static_cast<float>(i) + 0.9f * Math::g_random.Float(),
                    0.2f,
                    static_cast<float>(j) + 0.9f * Math::g_random.Float()
                };

                if ((center - Math::Vector3f(4, 0.2f, 0)).SquaredNorm() > 1.0f)
                {
                    const auto chooseMat = Math::g_random.Float();
                    if (chooseMat < 0.8f)
                    {
                        auto albedo = Color::Random() * Color::Random();
                        auto sphereMaterial = std::make_shared<Lambert>(albedo);
                        m_scene.Add(std::make_shared<Sphere>(center, 0.2f, sphereMaterial));
                    }
                    else if (chooseMat < 0.95f)
                    {
                        auto albedo = Color::Random(0.5f, 1.0f);
                        auto fuzz = Math::g_random.Float(0.0f, 0.5f);
                        auto sphere_material = std::make_shared<Metal>(albedo, fuzz);
                        m_scene.Add(std::make_shared<Sphere>(center, 0.2f, sphere_material));
                    }
                    else
                    {
                        auto sphere_material = std::make_shared<Dielectric>(1.5f);
                        m_scene.Add(std::make_shared<Sphere>(center, 0.2f, sphere_material));
                    }
                }
            }
        }

        auto material1 = std::make_shared<Dielectric>(1.5f);
        m_scene.Add(std::make_shared<Sphere>(Math::Vector3f(0, 1, 0), 1.0f, material1));

        auto material2 = std::make_shared<Lambert>(Color(0.4f, 0.2f, 0.1f));
        m_scene.Add(std::make_shared<Sphere>(Math::Vector3f(-4, 1, 0), 1.0f, material2));

        auto material3 = std::make_shared<Metal>(Color(0.7f, 0.6f, 0.5f), 0.0f);
        m_scene.Add(std::make_shared<Sphere>(Math::Vector3f(4, 1, 0), 1.0f, material3));
    }

    void PathTracer::Run(const PathTraceData& pathTraceData)
    {
        try
        {
            m_renderCompleted = false;

            LogInfo("Start path tracing");

            m_camera.Update(pathTraceData);

            m_camera.Render("image.bmp", m_scene, m_process);

            m_renderCompleted = true;
        }
        catch (const std::exception& e)
        {
            LogError(e.what());
            m_renderCompleted = true;
        }
        catch (...)
        {
            LogError("Unknown Exception");
            m_renderCompleted = true;
        }
    }
}
