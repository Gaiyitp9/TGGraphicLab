/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Rendering/RayTracing/Ray.h"

namespace TG
{
    Ray::Ray(const Math::Vector3f& origin, const Math::Vector3f& direction)
        : m_origin(origin), m_direction(direction) {}

    Math::Vector3f Ray::At(float t) const
    {
        return m_origin + t * m_direction;
    }
}
