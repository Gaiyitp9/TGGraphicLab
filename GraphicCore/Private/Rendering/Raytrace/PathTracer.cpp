/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Rendering/Raytrace/PathTracer.h"
#include "Math/Geometry/Intersection.h"
#include "Rendering/Color/StandardColors.h"
#include "Diagnostic/Log.hpp"
#include "stb_image_write.h"

namespace TG::Rendering
{
    void PathTracer::Run(const PathTraceData& pathTraceData)
    {
        m_renderCompleted = false;

        LogInfo("Start path tracing");

        constexpr int imageWidth = 1600;
        const int imageHeight = static_cast<int>(std::floor(static_cast<float>(imageWidth) /
            pathTraceData.aspectRatio));

        const float viewportHeight = pathTraceData.focalLength *
            std::tan(pathTraceData.fov * Math::Deg2RadF * 0.5f) * 2.0f;
        const float viewportWidth = viewportHeight *
            (static_cast<float>(imageWidth) / static_cast<float>(imageHeight));

        const Math::Vector3f viewportU = viewportWidth * pathTraceData.right;
        const Math::Vector3f viewportV = -viewportHeight * pathTraceData.up;

        const Math::Vector3f pixelDeltaU = viewportU / static_cast<float>(imageWidth);
        const Math::Vector3f pixelDeltaV = viewportV / static_cast<float>(imageHeight);

        const Math::Vector3f viewportUpperLeft = pathTraceData.cameraPosition + pathTraceData.front *
            pathTraceData.focalLength - (viewportU + viewportV) * 0.5f;
        const Math::Vector3f pixel00Location = viewportUpperLeft + (pixelDeltaU + pixelDeltaV) * 0.5f;

        const int imageDataSize = imageWidth * imageHeight * 3;
        std::vector<char> imageData(imageDataSize);
        for (int i = 0; i < imageHeight; ++i)
        {
            for (int j = 0; j < imageWidth; ++j)
            {
                Math::Vector3f pixelCenter = pixel00Location + static_cast<float>(j) * pixelDeltaU +
                    static_cast<float>(i) * pixelDeltaV;
                Math::Vector3f rayDirection = pixelCenter - pathTraceData.cameraPosition;
                Math::Geometry::Ray ray(pathTraceData.cameraPosition, rayDirection);
                Color pixelColor = RayColor(ray);

                const int offset = (imageWidth * i + j) * 3;
                imageData[offset] = static_cast<char>(pixelColor.R() * 255);
                imageData[offset + 1] = static_cast<char>(pixelColor.G() * 255);
                imageData[offset + 2] = static_cast<char>(pixelColor.B() * 255);

                m_process = static_cast<float>(offset) / static_cast<float>(imageDataSize);
            }
        }
        stbi_write_bmp("image.bmp", imageWidth, imageHeight, 3, imageData.data());

        m_renderCompleted = true;
    }

    Color PathTracer::RayColor(const Math::Geometry::Ray& ray) const
    {
        const Math::Geometry::Sphere sphere{ { 0.0f, 1.0f, -10.0f }, 1.0f };
        float tMin, tMax;
        if (Math::Geometry::Intersection::RaySphere(ray, sphere, tMin, tMax) && tMin > 0)
        {
            Math::Vector3f normal = (ray.At(tMin) - sphere.Center()) / sphere.Radius();
            return Color(normal.X() + 1.0f, normal.Y() + 1.0f, normal.Z() + 1.0f) * 0.5f;
        }

        const Math::Geometry::Plane plane{ { 0.0f, 0.01f, 0.0f }, { 0.0f, 1.0f, 0.0f } };
        const Math::Geometry::ConvexPolygon convex{
            plane,
            {
                { -10.0f, 0.01f, -20.0f },
                {  10.0f, 0.01f, -20.0f },
                {  10.0f, 0.01f,  0.0f },
                { -10.0f, 0.01f,  0.0f },
            }
        };
        float t;
        if (Math::Geometry::Intersection::RayConvexPolygon(ray, convex, t) && t > 0)
        {
            return { 0.0f, 1.0f, 0.0f };
        }

        const float a = 0.5f * (ray.Direction().Normalized().Y() + 1.0f);
        return a * Color(0.5f, 0.7f, 1.0f) + (1 - a) * White;
    }
}
