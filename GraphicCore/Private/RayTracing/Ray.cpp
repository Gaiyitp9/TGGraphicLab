/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "RayTracing/Ray.h"

namespace TG
{
    Ray::Ray(const Math::Vector3d& origin, const Math::Vector3d& direction)
        : m_origin(origin), m_direction(direction) {}

    Math::Vector3f Ray::At(double t) const
    {
        return m_origin + t * m_direction;
    }
}
