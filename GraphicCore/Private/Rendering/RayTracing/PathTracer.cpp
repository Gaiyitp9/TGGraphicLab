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

namespace TG::Rendering::RayTracing
{
    PathTracer::PathTracer()
    {
        auto materialGround = std::make_shared<Lambert>(Color(0.8f, 0.8f, 0.0f));
        auto materialCenter = std::make_shared<Lambert>(Color(0.1f, 0.2f, 0.5f));
        auto materialLeft = std::make_shared<Dielectric>(1.5f);
        auto materialBubble = std::make_shared<Dielectric>(1.0f / 1.50f);
        auto materialRight = std::make_shared<Metal>(Color(0.8f, 0.6f, 0.2f), 0.0f);
        m_scene.Add(std::make_shared<Sphere>(Math::Vector3f{ -6.0f, 1.0f, -10.0f }, 1.0f, materialLeft));
        m_scene.Add(std::make_shared<Sphere>(Math::Vector3f{ -3.0f, 1.0f, -10.0f }, 1.0f, materialLeft));
        m_scene.Add(std::make_shared<Sphere>(Math::Vector3f{ -3.0f, 1.0f, -10.0f }, 0.8f, materialBubble));
        m_scene.Add(std::make_shared<Sphere>(Math::Vector3f{  0.0f, 1.0f, -10.0f }, 1.0f, materialCenter));
        m_scene.Add(std::make_shared<Sphere>(Math::Vector3f{  3.0f, 1.0f, -10.0f }, 1.0f, materialRight));
        m_scene.Add(std::make_shared<Sphere>(Math::Vector3f{  6.0f, 1.0f, -10.0f }, 1.0f, materialCenter));
        m_scene.Add(std::make_shared<Plane>(
            Math::Vector3f{ 0.0f, 1.0f, 0.0f },
            std::vector<Math::Vector3f>
            {
                { -10.0f, 0.01f, -20.0f },
                {  10.0f, 0.01f, -20.0f },
                {  10.0f, 0.01f,  0.0f },
                { -10.0f, 0.01f,  0.0f }
            },
            materialGround
        ));
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
