/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/Camera.h"

namespace TG
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

    void RunPathTracer(std::atomic<float>& process, std::atomic<bool>& renderDone, PathTraceData pathTraceData);
}
