/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Reflection
{
    // 域列表
    template<typename... Fields>
    struct FieldList
    {
        constexpr static std::size_t Size = sizeof...(Fields);

        constexpr explicit FieldList(Fields... elems) : elems{elems...} {}

        // 遍历元素列表，并对每个元素调用指定函数
        // 传入的函数Func只有一个参数，并且能接收元素列表中的每个元素
        template<typename Func>
        requires requires(Func func) { (func(std::declval<Fields>()) , ...); }
        constexpr void ForEach(const Func& func) const
        {
            if constexpr (Size > 0)
                ForEach(func, std::make_index_sequence<Size>());
        }

        // 根据名称寻找元素
        // 传入的名称必须包含在列表内
        template<typename Name>
        constexpr std::size_t Find(const Name& name) const
        {
            if constexpr (Size > 0)
                return FindIf([&](const auto& field) { return field.name == name;}, std::make_index_sequence<Size>());
            return static_cast<std::size_t>(-1);
        }

        template<typename T>
        constexpr std::size_t FindValue(const T& value) const
        {
            if constexpr (Size > 0)
                return FindIf([&value](const auto& field) { return field.value == value;}, std::make_index_sequence<Size>());
            return static_cast<std::size_t>(-1);
        }

        // 判断是否包含指定名称的元素
        template<typename Name>
        constexpr bool Contains(Name name) const
        {
            for (std::size_t i = 0; i < Size; ++i)
            {
                if constexpr (Get<i>().EqualsName(name))
                    return true;
            }
            return false;
        }

        // 获取第N个元素
        template<std::size_t N>
        constexpr const auto& Get() const
        {
            return std::get<N>(elems);
        }

    private:
        template<typename Func, std::size_t N0, std::size_t... Ns>
        constexpr void ForEach(const Func& func, std::index_sequence<N0, Ns...>) const
        {
            func(Get<N0>());
            if constexpr (sizeof...(Ns) > 0)
                ForEach(func, std::index_sequence<Ns...>());
        }

        template<typename Predicate, std::size_t N0, std::size_t... Ns>
        constexpr std::size_t FindIf(const Predicate& func, std::index_sequence<N0, Ns...>) const
        {
            if (func(Get<N0>()))
                return N0;
            // 一定要用if constexpr，否则无法匹配到FindIF函数
            if constexpr (sizeof...(Ns) > 0)
                return FindIf(func, std::index_sequence<Ns...>());

            return static_cast<std::size_t>(-1);
        }

        std::tuple<Fields...> elems;
    };

    // 属性列表，和域列表是一样的，都是name-value pair组成的列表，所以功能也一样，使用别名提高可读性
    template<typename... Attributes>
    using AttributeList = FieldList<Attributes...>;
}