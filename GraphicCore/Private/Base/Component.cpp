/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Base/Component.h"

namespace TG
{
    Component::Component(Object *owner) : m_owner(owner)
    {

    }

    Component::~Component() = default;
}
