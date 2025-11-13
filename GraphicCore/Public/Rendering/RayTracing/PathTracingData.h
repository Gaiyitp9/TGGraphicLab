/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Core.hpp"

namespace TG::Rendering::RayTracing
{
    struct PathTraceData
    {
        unsigned int imageWidth;
        unsigned int imageHeight;
        Math::Vector3f cameraPosition;
        Math::Vector3f front;
        Math::Vector3f up;
        Math::Vector3f right;
        float aspectRatio;
        float focalDistance;
        float fov;
        float defocusAngle;
        unsigned int samplesPerPixel;
        unsigned int maxDepth;
    };
}
