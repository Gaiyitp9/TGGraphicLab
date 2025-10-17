/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Rendering/RayTracing/PathTracer.h"
#include "Diagnostic/Log.hpp"
#include "Rendering/RayTracing/Ray.h"
#include "Color/Color.h"
#include "stb_image_write.h"
#include "Color/StandardColors.h"

namespace TG
{
    Color::Color RayColor(const Ray& ray)
    {
        const float a = 0.5f * (ray.Direction().Normalized().Y() + 1.0f);
        return a * Color::Color(0.5, 0.7, 1.0) + (1 - a) * Color::White;
    }

    void RunPathTracer(std::atomic<float>& process, std::atomic<bool>& renderDone, PathTraceData pathTraceData)
    {
        LogInfo("Start path tracing");
        renderDone = false;

        constexpr int imageWidth = 400;
        const int imageHeight = static_cast<int>(std::floor(static_cast<float>(imageWidth) /
            pathTraceData.aspectRatio));

        const float viewportHeight = pathTraceData.focalLength *
            std::tan(pathTraceData.fov * Math::Deg2RadF * 0.5f) * 2.0f;
        const float viewportWidth = viewportHeight *
            (static_cast<float>(imageWidth) / static_cast<float>(imageHeight));

        Math::Vector3f viewportU = viewportWidth * pathTraceData.right;
        Math::Vector3f viewportV = -viewportHeight * pathTraceData.up;

        Math::Vector3f pixelDeltaU = viewportU / static_cast<float>(imageWidth);
        Math::Vector3f pixelDeltaV = viewportV / static_cast<float>(imageHeight);

        Math::Vector3f viewportUpperLeft = pathTraceData.cameraPosition + pathTraceData.front *
            pathTraceData.focalLength - (viewportU + viewportV) * 0.5f;
        Math::Vector3f pixel00Location = viewportUpperLeft + (pixelDeltaU + pixelDeltaV) * 0.5f;

        const int imageDataSize = imageWidth * imageHeight * 3;
        std::vector<char> imageData(imageDataSize);
        for (int i = 0; i < imageHeight; ++i)
        {
            for (int j = 0; j < imageWidth; ++j)
            {
                Math::Vector3f pixelCenter = pixel00Location + static_cast<float>(j) * pixelDeltaU +
                    static_cast<float>(i) * pixelDeltaV;
                Math::Vector3f rayDirection = pixelCenter - pathTraceData.cameraPosition;
                Ray ray(pathTraceData.cameraPosition, rayDirection);
                Color::Color pixelColor = RayColor(ray);

                int offset = (imageWidth * i + j) * 3;
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
