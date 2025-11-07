/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Base/Component.h"
#include "Math/Geometry/Ray.h"
#include "Physics/HitInfo.h"

namespace TG
{
    class ColliderComponent : public Component
    {
    public:
        ColliderComponent();

        virtual bool Raycast(const Math::Geometry::Ray& ray, HitInfo& hitInfo) const = 0;
    };
}
