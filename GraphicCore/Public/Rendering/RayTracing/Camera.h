/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Geometry/Ray.h"
#include "Rendering/RayTracing/Scene.h"
#include "Rendering/RayTracing/PathTracingData.h"

namespace TG::Rendering::RayTracing
{
    class Camera
    {
    public:
        Camera() = default;
        Camera(unsigned int imageWidth, Math::Vector3f cameraPosition, float aspectRatio, float focalDistance,
            float fov, float defocusAngle, Math::Vector3f front, Math::Vector3f up, Math::Vector3f right,
            unsigned int samplesPerPixel, unsigned int maxDepth);

        void Render(char const* imageFile, const Scene& scene, float& process);

        void Update(const PathTraceData& data);

    private:
        [[nodiscard]] Math::Geometry::Ray GetRay(unsigned int row, unsigned int column) const;

        unsigned int m_imageWidth = 400;
        unsigned int m_imageHeight = 225;

        Math::Vector3f m_position;
        Math::Vector3f m_front;
        Math::Vector3f m_up;
        Math::Vector3f m_right;

        float m_aspectRatio = 16.0f / 9.0f;
        float m_focalDistance = 1.0f;
        float m_fov = 45.0f;
        float m_defocusAngle = 10.0f;
        float m_viewportHeight = m_focalDistance * std::tan(m_fov * Math::Deg2RadF * 0.5f) * 2.0f;
        float m_viewportWidth = m_viewportHeight * m_aspectRatio;
        Math::Vector3f m_viewportU{ m_viewportWidth, 0, 0 };
        Math::Vector3f m_viewportV{ 0, -m_viewportHeight, 0 };
        Math::Vector3f m_pixelDeltaU{ m_viewportU / static_cast<float>(m_imageWidth) };
        Math::Vector3f m_pixelDeltaV{ m_viewportV / static_cast<float>(m_imageHeight) };
        Math::Vector3f m_viewportUpperLeft{
            m_position - Math::Vector3f(0.0, 0.0, m_focalDistance) - 0.5 * (m_viewportU + m_viewportV)
        };
        Math::Vector3f m_pixel00Location{ m_viewportUpperLeft + 0.5 * (m_pixelDeltaU + m_pixelDeltaV) };
        Math::Vector3f m_defocusDiskU{ m_focalDistance * std::tan(m_defocusAngle * Math::Deg2RadF * 0.5f), 0, 0 };
        Math::Vector3f m_defocusDiskV{ 0, -m_focalDistance * std::tan(m_defocusAngle * Math::Deg2RadF * 0.5f), 0};

        unsigned int m_samplesPerPixel = 10;
        unsigned int m_maxDepth = 50;
    };
}
