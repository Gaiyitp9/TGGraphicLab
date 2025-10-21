/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Mesh.h"

namespace TG::Rendering
{
    class Plane : public Mesh
    {
    public:
        Plane();
        Plane(float width, float height, int rows, int columns);
        ~Plane();

        void Regenerate();

    private:
        float m_width{ 1.0f };
        float m_height{ 1.0f };
        unsigned int m_rows{ 1 };
        unsigned int m_columns{ 1 };
    };
}
