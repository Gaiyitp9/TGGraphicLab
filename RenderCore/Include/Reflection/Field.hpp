/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Reflection
{
    // 域特性
    template<typename Object, typename Value, bool IsStatic_, bool IsFunction_>
    struct FieldTraitsBase
    {
        using ObjectType = Object;
        using ValueType = Value;
        static constexpr bool IsStatic = IsStatic_;
        static constexpr bool IsFunction = IsFunction_;
    };
    // 域是枚举或static constexpr变量
    template<typename T>
    struct FieldTraits : FieldTraitsBase<void , T, true, false> {};
    // 域是类(或结构体)的普通成员
    template<typename Object, typename T>
    struct FieldTraits<T Object::*> : FieldTraitsBase<Object, T, false, std::is_function_v<T>> {};
    // 域是类(或结构体)的静态成员
    template<typename T>
    struct FieldTraits<T*> : FieldTraitsBase<void, T, true, std::is_function_v<T>> {};

    // 检查T是否是U的一个实例
    template<typename T, template<typename...>class U>
    struct IsInstance : std::false_type {};
    template<template<typename...>class U, typename... Ts>
    struct IsInstance<U<Ts...>, U> : std::true_type {};

    // 属性列表要用AttributeList模板类来保存
    template<typename T>
    concept AttributeListInstance = IsInstance<T, AttributeList>::value;

    // 域
    template<typename Name, typename T, typename AList = AttributeList<>> requires AttributeListInstance<AList>
    struct Field : NameValuePair<Name, T>
    {
        constexpr Field(Name name, T value, AList attrs = AttributeList{})
            : NameValuePair<Name, T>(name, value) {}

        AList attrs;
    };
}
