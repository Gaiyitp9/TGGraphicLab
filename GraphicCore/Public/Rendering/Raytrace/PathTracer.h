/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Geometry/Ray.h"
#include "Rendering/Color/Color.h"
#include <atomic>

namespace TG::Rendering
{
    struct PathTraceData
    {
        float aspectRatio;
        float focalLength;
        float fov;
        Math::Vector3f cameraPosition;
        Math::Vector3f front;
        Math::Vector3f up;
        Math::Vector3f right;
    };

    class PathTracer
    {
    public:
        PathTracer() = default;
        ~PathTracer() = default;

        void Run(const PathTraceData& pathTraceData);

        bool IsReady() const { return m_renderCompleted; }
        float Process() const { return m_process; }

    private:
        Color RayColor(const Math::Geometry::Ray& ray) const;

        float m_process{ 0 };
        std::atomic_bool m_renderCompleted{ true };
    };
}
