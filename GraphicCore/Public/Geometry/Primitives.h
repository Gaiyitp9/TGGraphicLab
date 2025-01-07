/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Geometry/Mesh.h"

namespace TG::Geometry
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

    template<PrimitiveType T> Mesh CreatePrimitive();
}
