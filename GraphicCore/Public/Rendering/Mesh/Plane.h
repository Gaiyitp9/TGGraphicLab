/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Mesh.h"

namespace TG::Rendering
{
    class Plane
    {
    public:
        Plane();

    private:
        Mesh m_mesh;

        float m_width;
        float m_height;
        unsigned int m_rows;
        unsigned int m_columns;
    };
}
