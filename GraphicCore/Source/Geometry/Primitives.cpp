/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Geometry/Primitives.h"

namespace TG::Geometry
{

    template<>
    Mesh CreatePrimitive<PrimitiveType::Quad>()
    {
        Mesh mesh;
        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.5f,  0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f,
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,
        };
        return mesh;
    }
}
