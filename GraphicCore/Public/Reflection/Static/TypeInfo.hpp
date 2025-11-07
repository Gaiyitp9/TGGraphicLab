/****************************************************************
* TianGong GraphicLab										*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Reflection
{
    template<typename T>
    struct TypeInfo
    {
        struct InvalidMarker {};

        template<std::size_t, typename>
        struct member;

        static constexpr std::size_t memberCount{ 0 };

        static constexpr ConstString name{};

        static constexpr std::tuple<> attributes{};
    };

    template<typename T>
    struct TypeInfo<const T> : TypeInfo<T> {};

    template<typename T>
    struct TypeInfo<volatile T> : TypeInfo<T> {};

    template<typename T>
    struct TypeInfo<const volatile T> : TypeInfo<T> {};
}
