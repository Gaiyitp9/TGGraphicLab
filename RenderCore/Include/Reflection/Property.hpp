/****************************************************************
* TianGong GraphicLab										*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Reflection
{
    template<typename Name, typename Field>
    class Property
    {
    public:
        constexpr Property(Name name, Field&& field)
            : m_name(name), m_field(std::forward<Field>(field)) {}

        template<typename Instance>
        constexpr auto GetValue(Instance&& instance)
        {
            return std::invoke(m_field, std::forward<Instance>(instance));
        }

        template<typename Instance, typename Value>
        constexpr void SetValue(Instance&& instance, Value&& value)
        {
            std::invoke(m_field, std::forward<Instance>(instance)) = std::forward<Value>(value);
        }

    private:
//        Specifier m_specifier;
        Name m_name;
        Field m_field;
    };
}
