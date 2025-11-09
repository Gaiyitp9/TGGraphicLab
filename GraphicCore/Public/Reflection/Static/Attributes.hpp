/****************************************************************
* TianGong GraphicLab										*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Reflection::Attribute
{
    namespace Usage
    {
        /**
         * \brief 继承该类的属性用于描述类
         */
        struct Type {};

        /**
         * \brief 继承该类的属性用于描述函数
         */
        struct Function {};

        /**
         * \brief 继承该类的属性用于描述类字段
         */
        struct Field {};

        /**
         * \brief 继承该类的属性用于描述类成员
         */
        struct Member : Function, Field {};

        /**
         * \brief 继承该类的属性用于类、函数和字段
         */
        struct Any : Member, Type {};
    }

    /**
     * \brief 函数标记为属性，设置别名
     */
    struct Property : Usage::Function
    {
        constexpr Property() = default;

        explicit constexpr Property(const char* friendlyName) : friendlyName(friendlyName) {}

        const std::optional<const char*> friendlyName{};
    };

    template<typename F>
    struct Debug : Usage::Any
    {
        explicit constexpr Debug(F write) : write(write) {}

        F write;
    };

    /**
     * \brief 用于记录基类的属性
     */
    template<typename... Ts>
    struct BaseTypes : Usage::Type
    {
        using List = TypeList<Ts...>;
    };
}
