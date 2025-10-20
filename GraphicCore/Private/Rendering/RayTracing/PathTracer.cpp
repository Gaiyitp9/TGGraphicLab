/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Rendering/RayTracing/PathTracer.h"
#include "Math/Geometry/Shape/Ray.h"
#include "Math/Geometry/Intersection/Intersection.h"
#include "Rendering/Color/Color.h"
#include "Rendering/Color/StandardColors.h"
#include "Diagnostic/Log.hpp"
#include "stb_image_write.h"

namespace TG
{
    Rendering::Color RayColor(const Math::Geometry::Ray& ray)
    {
        const Math::Geometry::Sphere sphere{ { 0.0f, 1.0f, -10.0f }, 1.0f };
        float tMin, tMax;
        if (Math::Geometry::RaySphereIntersection(ray, sphere, tMin, tMax) && tMin > 0)
        {
            Math::Vector3f normal = (ray.At(tMin) - sphere.Center()) / sphere.Radius();
            return Rendering::Color(normal.X() + 1.0f, normal.Y() + 1.0f, normal.Z() + 1.0f) * 0.5f;
        }

        const float a = 0.5f * (ray.Direction().Normalized().Y() + 1.0f);
        return a * Rendering::Color(0.5, 0.7, 1.0) + (1 - a) * Rendering::White;
    }

    void RunPathTracer(std::atomic<float>& process, std::atomic<bool>& renderDone, PathTraceData pathTraceData)
    {
        LogInfo("Start path tracing");
        renderDone = false;

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
                Rendering::Color pixelColor = RayColor(ray);

                const int offset = (imageWidth * i + j) * 3;
                imageData[offset] = static_cast<char>(pixelColor.R() * 255);
                imageData[offset + 1] = static_cast<char>(pixelColor.G() * 255);
                imageData[offset + 2] = static_cast<char>(pixelColor.B() * 255);

                process = static_cast<float>(offset) / static_cast<float>(imageDataSize);
            }
        }
        stbi_write_bmp("image.bmp", imageWidth, imageHeight, 3, imageData.data());

        renderDone = true;
    }
}
