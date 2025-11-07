/****************************************************************
* TianGong GraphicLab										*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Reflection
{
    /**
     * \brief 判断类型T是否能反射
     */
    template<typename T>
    concept IsReflectable = !requires
    {
        typename TypeInfo<T>::InvalidMarker;
    };

    /**
     * \brief 判断类型T是否是容器
     */
    template<typename T>
    concept IsContainer = requires(T t)
    {
        { std::begin(t) } -> std::input_or_output_iterator;
        { std::end(t) } -> std::input_or_output_iterator;
    };

    namespace Details
    {
        template<typename T>
        struct IsInstance : std::false_type {};
        template<template<typename...> class T, typename... Args>
        struct IsInstance<T<Args...>> : std::true_type {};

        template<template<typename...> class T, typename U>
        struct IsInstanceOf : std::false_type {};
        template<template<typename...> class T, typename... Args>
        struct IsInstanceOf<T, T<Args...>> : std::true_type {};

        template<typename T>
        struct AsTypeList;
        template<template<typename...> class T, typename... Args>
        struct AsTypeList<T<Args...>>
        {
            using Type = TypeList<Args...>;
        };
        template<typename T>
        struct AsTypeList : AsTypeList<std::remove_cvref_t<T>> {};

        template<typename T>
        struct AsTuple;
        template<template<typename...> class T, typename... Args>
        struct AsTuple<T<Args...>>
        {
            using Type = TypeList<Args...>;
        };
        template<typename T>
        struct AsTuple : AsTuple<std::remove_cvref_t<T>> {};
    }

    /**
     * \brief 检查类型T是否是一个实例
     */
    template<typename T>
    static constexpr bool IsInstance = Details::IsInstance<T>::value;
    /**
     * \brief 检测类型T是否是U的一个实例
     */
    template<template<typename...> class T, typename U>
    static constexpr bool IsInstanceOf = Details::IsInstanceOf<T, U>::value;
    /**
     * \brief 萃取模板参数列表并转换为TypeList
     */
    template<typename T>
    using AsTypeList = Details::AsTypeList<T>::Type;
    /**
     * \brief 萃取模板参数列表并转换为tuple
     */
    template<typename T>
    using AsTuple = Details::AsTuple<T>::Type;
}
