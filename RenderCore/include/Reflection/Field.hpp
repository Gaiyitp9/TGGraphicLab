/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Reflection
{
    // 字段特性
    template<typename Object, typename Value, bool IsStatic_, bool IsFunction_>
    struct FieldTraitsBase
    {
        using ObjectType = Object;
        using ValueType = Value;
        static constexpr bool IsStatic = IsStatic_;
        static constexpr bool IsFunction = IsFunction_;
    };
    // 字段是枚举或static constexpr变量
    template<typename T>
    struct FieldTraits : FieldTraitsBase<void , T, true, false> {};
    // 字段是类(或结构体)的普通成员
    template<typename Object, typename T>
    struct FieldTraits<T Object::*> : FieldTraitsBase<Object, T, false, std::is_function_v<T>> {};
    // 字段是类(或结构体)的静态成员
    template<typename T>
    struct FieldTraits<T*> : FieldTraitsBase<void, T, true, std::is_function_v<T>> {};

    // 检查T是否是U的一个实例
    template<typename T, template<typename...>class U>
    struct IsInstance : std::false_type {};
    template<template<typename...>class U, typename... Ts>
    struct IsInstance<U<Ts...>, U> : std::true_type {};

    // 字段相关的概念。字段值不能为void，同时字段属性列表要用AttrList模板类来保存
    template<typename T>
    concept AttrListInstance = IsInstance<T, AttrList>::value;
    template<typename T>
    concept FieldValue = !std::is_void_v<T>;

    template<typename Name, FieldValue T, AttrListInstance AList = AttrList<>>
    struct Field : NamedValue<Name, T>
    {
        constexpr Field(Name name, T value, AList attrs = {})
            : NamedValue<Name, T>(name, value), attrs(attrs) {}

        AList attrs;
    };
}
