/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Physics/ColliderComponent.h"

namespace TG
{
    class SphereCollider : public ColliderComponent
    {
    public:
        SphereCollider();

        bool Raycast(const Math::Geometry::Ray &ray, HitInfo &hitInfo) const override;
    };
}
