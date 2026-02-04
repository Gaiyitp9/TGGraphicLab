/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Base/Object.h"
#include "Base/Component.h"

namespace TG
{
    Object::Object() = default;

    Object::~Object() = default;

    void Object::Update(float deltaTime) const
    {
        for (auto& component : m_components)
            component->Update(deltaTime);
    }
}
