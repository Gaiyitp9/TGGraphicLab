/****************************************************************
* TianGong GraphicLab										*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Reflection
{
    template<typename... Types> struct TypeList;

    namespace Detail
    {
        template<typename T>
        struct IsInstance : std::false_type {};
        template<template<typename...> typename T, typename... Args>
        struct IsInstance<T<Args...>> : std::true_type {};

        template<template<typename...> typename T, typename U>
        struct IsInstanceOf : std::false_type {};
        template<template<typename...> typename T, typename... Args>
        struct IsInstanceOf<T, T<Args...>> : std::true_type {};

        template<typename T>
        struct AsTypeList;
        template<template<typename...> typename T, typename... Args>
        struct AsTypeList<T<Args...>>
        {
            using Type = TypeList<Args...>;
        };
        template<typename T>
        struct AsTypeList : AsTypeList<std::remove_cvref_t<T>> {};

        template<typename T>
        struct AsTuple;
        template<template<typename...> typename T, typename... Args>
        struct AsTuple<T<Args...>>
        {
            using Type = std::tuple<Args...>;
        };
        template<typename T>
        struct AsTuple : AsTuple<std::remove_cvref_t<T>> {};

        template<typename T, typename U>
        struct TransferConst
        {
            using Type = std::conditional_t<std::is_const_v<T>, std::add_const_t<U>, U>;
        };
        template<typename T, typename U>
        struct TransferVolatile
        {
            using Type = std::conditional_t<std::is_volatile_v<T>, std::add_volatile_t<U>, U>;
        };
        template<typename T, typename U>
        struct TransferCV : TransferConst<T, typename TransferVolatile<T, U>::Type>
        {};
        template<typename T, typename U>
        struct TransferLvalueRef
        {
            using Type = std::conditional_t<std::is_lvalue_reference_v<T>, std::add_rvalue_reference_t<U>, U>;
        };
        template<typename T, typename U>
        struct TransferRvalueRef
        {
            using Type = std::conditional_t<std::is_rvalue_reference_v<T>, std::add_rvalue_reference_t<U>, U>;
        };
        template<typename T, typename U>
        struct TransferRef : TransferRvalueRef<T, typename TransferLvalueRef<T, U>::Type>
        {};
        template<typename T, typename U>
        struct TransferCVRef : TransferRef<T, typename TransferCV<std::remove_reference_t<T>, U>::Type>
        {};
        template<typename T, typename U>
        constexpr auto ForwardCast(std::remove_reference_t<T>& t)
        {
            return static_cast<TransferCVRef<T, U>::Type&&>(t);
        }
        template<typename T, typename U>
        constexpr auto ForwardCast(std::remove_reference_t<T>&& t)
        {
            return static_cast<TransferCVRef<T, U>::Type&&>(t);
        }
    }

    template<template<typename> typename Trait, typename T>
    concept IsCustomTypeTrait = requires { typename Trait<T>::Type; } || requires { typename Trait<T>::Value; };

    /**
     * \brief 检查类型T是否是一个实例
     */
    template<typename T>
    static constexpr bool IsInstance = Detail::IsInstance<T>::value;
    /**
     * \brief 检测类型U是否是T的一个实例
     */
    template<template<typename...> typename T, typename U>
    static constexpr bool IsInstanceOf = Detail::IsInstanceOf<T, U>::value;
    /**
     * \brief 萃取模板参数列表并转换为TypeList
     */
    template<typename T>
    using AsTypeList = Detail::AsTypeList<T>::Type;
    /**
     * \brief 萃取模板参数列表并转换为tuple
     */
    template<typename T>
    using AsTuple = Detail::AsTuple<T>::Type;

    /**
     * \brief 判断类型T是否是容器
     */
    template<typename T>
    concept IsContainer = requires(T t)
    {
        { std::begin(t) } -> std::input_or_output_iterator;
        { std::end(t) } -> std::input_or_output_iterator;
    };
}
