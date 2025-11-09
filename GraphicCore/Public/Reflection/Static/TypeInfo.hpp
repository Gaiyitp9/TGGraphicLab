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

        template<std::size_t>
        struct Member;

        static constexpr std::size_t MemberCount{ 0 };

        static constexpr ConstString Name{};

        static constexpr std::tuple<> Attributes{};
    };

    template<typename T>
    struct TypeInfo<const T> : TypeInfo<T> {};

    template<typename T>
    struct TypeInfo<volatile T> : TypeInfo<T> {};

    template<typename T>
    struct TypeInfo<const volatile T> : TypeInfo<T> {};

    template<typename T, std::size_t N>
    using MemberInfo = TypeInfo<T>::template Member<N>;
}
