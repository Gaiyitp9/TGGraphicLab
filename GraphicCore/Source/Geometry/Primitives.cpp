/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Geometry/Primitives.h"

namespace TG::Geometry
{

    template<>
    Mesh CreatePrimitive<PrimitiveType::Sphere>()
    {
        Mesh mesh;
        return mesh;
    }
}
