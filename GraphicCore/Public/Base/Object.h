/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <memory>
#include <vector>

namespace TG
{
    class Component;

    class Object
    {
    public:
        Object();
        ~Object();

        template<typename T, typename... Args> requires std::is_base_of_v<Component, T>
        T* AddComponent(Args&&... args)
        {
            auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
            T* ptr = component.get();
            m_components.push_back(std::move(component));
            return ptr;
        }

        template<typename T>
        T* GetComponent()
        {
            for (auto& component : m_components)
            {
                if (auto ptr = dynamic_cast<T*>(component.get()))
                    return ptr;
            }
            return nullptr;
        }

        void Update(float deltaTime) const;

    private:
        std::vector<std::unique_ptr<Component>> m_components;
    };
}
