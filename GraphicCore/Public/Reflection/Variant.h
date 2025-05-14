/****************************************************************
* TianGong GraphicLab										    *
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <any>

namespace TG::Reflection
{
    class Variant
    {
    public:
        Variant();
        ~Variant();

        template<typename T>
        Variant(T&& value) : m_data(std::forward<T>(value)) {}

        template<typename T>
        Variant& operator=(T&& value)
        {
            m_data = std::forward<T>(value);
            return *this;
        }

        bool operator==(const Variant& other) const;

    private:
        std::any m_data;
    };
}
