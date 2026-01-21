/****************************************************************
* TianGong GraphicLab										    *
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Reflection::Util
{
    template<typename... Ts>
    constexpr TypeList<Ts...> AsTypeList(const std::tuple<Ts...>&) noexcept
    {
        return {};
    }

    template<typename... Ts>
    constexpr std::tuple<Ts...> AsTuple(TypeList<Ts...>) noexcept
    {
        return {};
    }

    template<template<typename...> typename T, typename... Ts> requires (... || IsInstanceOf<T, Ts>)
    constexpr auto& GetInstance(std::tuple<Ts...>& ts)
    {
        constexpr auto index = static_cast<std::size_t>(TypeList<Ts...>::template IndexOfInstance<T>());
        return std::get<index>(ts);
    }

    template<template<typename...> typename T, typename... Ts> requires (... || IsInstanceOf<T, Ts>)
    constexpr const auto& GetInstance(const std::tuple<Ts...>& ts)
    {
        constexpr auto index = static_cast<std::size_t>(TypeList<Ts...>::template IndexOfInstance<T>());
        return std::get<index>(ts);
    }
}
