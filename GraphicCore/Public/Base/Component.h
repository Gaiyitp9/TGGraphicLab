/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <string>

namespace TG
{
    class Object;

    class Component
    {
    public:
        explicit Component(Object* owner);
        virtual ~Component();

        virtual void Update(float deltaTime) = 0;
        [[nodiscard]] virtual std::string GetName() const = 0;

    private:
        Object* m_owner;
    };
}
