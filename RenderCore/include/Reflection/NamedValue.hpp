/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Reflection
{
    template<typename Name, typename T>
    struct NamedValueBase
    {
        static constexpr bool HasValue = !std::is_void_v<T>;

        template<typename Str>
        static constexpr bool NameIs() noexcept
        {
            return std::is_same_v<Name, Str>;
        }

        template<typename U>
        static constexpr bool ValueTypeIs() noexcept
        {
            return std::is_same_v<T, U>;
        }
    };

    template<typename Name, typename T>
    struct NamedValue : NamedValueBase<Name, T>
    {
        constexpr NamedValue(Name, T value) : value(value) {}

        template<typename U>
        constexpr bool operator==(U v) const
        {
            // 只有T和U的类型相同时才可以比较
            if constexpr (std::is_same_v<T, U>)
                return value == v;
            else
                return false;
        }

        T value;
    };

    // void不可以作为类型，NamedValue(Name, void value)和void value都是非法的，所以当T是void时需要实现一个偏特化类
    template<typename Name>
    struct NamedValue<Name, void> : NamedValueBase<Name, void>
    {
        constexpr explicit NamedValue(Name) {}

        template<typename U>
        constexpr bool operator==(U) const { return false; }
    };

    template<typename Name>
    NamedValue(Name)->NamedValue<Name, void>;
}