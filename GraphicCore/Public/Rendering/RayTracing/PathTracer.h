/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/RayTracing/Camera.h"
#include <atomic>

namespace TG::Rendering::RayTracing
{
    class PathTracer
    {
    public:
        PathTracer();
        ~PathTracer() = default;

        void Run(const PathTraceData& pathTraceData);

        bool IsReady() const { return m_renderCompleted; }
        float Process() const { return m_process; }

    private:
        Camera m_camera;
        Scene m_scene;

        float m_process{ 0 };
        std::atomic_bool m_renderCompleted{ true };
    };
}
