/****************************************************************
* TianGong GraphicLab										*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <type_traits>

namespace TG
{
    template<typename... Types>
    struct TypeList
    {
        static constexpr std::size_t Size = sizeof...(Types);
        // 是否包含类型T
        template<typename T> static constexpr bool Contains = (std::is_same_v<T, Types> || ...);
    };

    // 检查类T是否是模板类U的一个实例
    template<typename T, template<typename...>class U>
    struct IsInstanceOf : std::false_type {};
    template<template<typename...>class U, typename... Ts>
    struct IsInstanceOf<U<Ts...>, U> : std::true_type {};
    template<typename T, template<typename...>class U>
    constexpr bool IsInstanceOfV = IsInstanceOf<T, U>::value;
}
