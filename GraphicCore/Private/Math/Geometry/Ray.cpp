/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Math/Geometry/Ray.h"

namespace TG::Math::Geometry
{
    Ray::Ray(const Vector3f &origin, const Vector3f &direction)
        : m_origin(origin), m_direction(direction)
    {

    }

    Vector3f Ray::Origin() const
    {
        return m_origin;
    }

    Vector3f Ray::Direction() const
    {
        return m_direction;
    }

    Vector3f Ray::At(float t) const
    {
        return m_origin + t * m_direction;
    }
}
