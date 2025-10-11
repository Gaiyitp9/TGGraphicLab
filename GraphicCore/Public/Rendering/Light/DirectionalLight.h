/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Light.h"

namespace TG::Rendering
{
    class DirectionalLight final : public Light
    {
    public:
        DirectionalLight();
        ~DirectionalLight() override;

        [[nodiscard]] LightType GetType() const override { return LightType::Directional; }

    private:
        Math::Vector3f m_lightPosition;
        Math::Vector3f m_lightDirection;
        Color::Color m_lightColor;
    };
}
