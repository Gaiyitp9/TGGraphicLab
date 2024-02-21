/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Reflection
{
    template<typename Name, typename Value>
    struct NameValuePairBase
    {
        constexpr static bool HasValue = !std::is_void_v<Value>;

        constexpr explicit NameValuePairBase(Name name) : name(name) {}

        template<typename T>
        constexpr bool NameEquals(T n) noexcept
        {
            return name == n;
        }

        Name name;
    };

    template<typename Name, typename Value>
    struct NameValuePair : NameValuePairBase<Name, Value>
    {
        constexpr NameValuePair(Name name, Value value) : NameValuePairBase<Name, Value>(name), value(value) {}

        template<typename T>
        constexpr bool ValueEquals(T v) const
        {
            // 只有值类型相同时才可以比较
            if constexpr (std::is_same_v<Value, T>)
                return value == v;
            else
                return false;
        }

        Value value;
    };

    // void不可以作为类型，NameValuePair(Name, void value)和void value都是非法的，所以当T是void时需要实现一个偏特化类
    template<typename Name>
    struct NameValuePair<Name, void> : NameValuePairBase<Name, void>
    {
        constexpr explicit NameValuePair(Name name) : NameValuePairBase<Name, void>(name) {}

        template<typename T>
        constexpr bool operator==(T) const { return false; }
    };

    template<typename Name>
    NameValuePair(Name)->NameValuePair<Name, void>;
}