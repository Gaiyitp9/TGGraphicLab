/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Mesh.h"

namespace TG::Rendering
{
    enum class PrimitiveType : unsigned char
    {
        Quad,
        Plane,
        Sphere,
        Cube,
        Capsule,
        Cylinder,
    };

    Mesh CreatePrimitive(PrimitiveType type);
}
