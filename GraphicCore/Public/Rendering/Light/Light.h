/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Color/Color.h"
#include "Rendering/Shader.h"

namespace TG::Rendering
{
    enum class LightType : unsigned char
    {
        Directional,
        Point,
        Spot,
        Sphere,
        Area,
    };

    class Light
    {
    public:
        Light();
        virtual ~Light();

        virtual LightType GetType() const = 0;

    private:
        Color::Color m_color;
        float m_intensity;
    };
}
