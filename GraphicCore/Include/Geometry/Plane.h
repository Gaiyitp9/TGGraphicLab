/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Geometry
{
    class Plane
    {
    public:
        Plane();

    private:
        float m_width;
        float m_height;
        int m_rows;
        int m_columns;
    };
}
