/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "RayTracing/PathTracer.h"
#include "Diagnostic/Log.hpp"
#include "stb_image_write.h"

namespace TG
{
    void RunPathTracer(std::atomic<float>& process, std::atomic<bool>& renderDone)
    {
        LogInfo("Start path tracing");
        renderDone = false;

        constexpr int imageWidth = 256, imageHeight = 256;
        constexpr int imageSize = imageWidth * imageHeight;
        char imageData[imageSize * 3];
        for (int i = 0; i < imageHeight; ++i)
        {
            for (int j = 0; j < imageWidth; ++j)
            {
                int pixelIndex = imageWidth * i + j;
                int offset = pixelIndex * 3;
                float r = static_cast<float>(j) / (imageWidth - 1);
                float g = static_cast<float>(i) / (imageHeight - 1);
                float b = 0.0f;
                imageData[offset] = static_cast<char>(r * 255);
                imageData[offset + 1] = static_cast<char>(g * 255);
                imageData[offset + 2] = static_cast<char>(b * 255);

                process = static_cast<float>(pixelIndex) / imageSize;
            }
        }
        stbi_flip_vertically_on_write(true);
        stbi_write_bmp("image.bmp", imageWidth, imageHeight, 3, imageData);

        renderDone = true;
    }
}
