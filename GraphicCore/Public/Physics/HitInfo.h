/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Core.hpp"

namespace TG
{
    struct HitInfo
    {
        Math::Vector3f  position;
        Math::Vector3f  normal;
        Math::Vector3f  tangent;
        Math::Vector3f  binormal;
        bool            frontFace = true;
    };
}
