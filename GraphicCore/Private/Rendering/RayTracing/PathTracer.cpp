/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Rendering/RayTracing/PathTracer.h"
#include "Diagnostic/Log.hpp"
#include "Rendering/RayTracing/Sphere.h"
#include "Rendering/RayTracing/Plane.h"

namespace TG::Rendering::RayTracing
{
    PathTracer::PathTracer()
    {
        m_scene.Add(std::make_shared<Sphere>(Math::Vector3f{ -6.0f, 1.0f, -10.0f }, 1.0f));
        m_scene.Add(std::make_shared<Sphere>(Math::Vector3f{ -3.0f, 1.0f, -10.0f }, 1.0f));
        m_scene.Add(std::make_shared<Sphere>(Math::Vector3f{  0.0f, 1.0f, -10.0f }, 1.0f));
        m_scene.Add(std::make_shared<Sphere>(Math::Vector3f{  3.0f, 1.0f, -10.0f }, 1.0f));
        m_scene.Add(std::make_shared<Sphere>(Math::Vector3f{  6.0f, 1.0f, -10.0f }, 1.0f));
        m_scene.Add(std::make_shared<Plane>(
            Math::Vector3f{ 0.0f, 1.0f, 0.0f },
            std::vector<Math::Vector3f>
            {
                { -10.0f, 0.01f, -20.0f },
                {  10.0f, 0.01f, -20.0f },
                {  10.0f, 0.01f,  0.0f },
                { -10.0f, 0.01f,  0.0f }
            }
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
