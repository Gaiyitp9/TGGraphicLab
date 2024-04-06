/****************************************************************
* TianGong RenderLab										*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Reflection
{
    template<typename... Elems>
    struct ElementList
    {
        constexpr static std::size_t Size = sizeof...(Elems);

        constexpr explicit ElementList(Elems... elems) : elems{elems...} {}

        // 获取第N个元素
        template<std::size_t N>
        constexpr const auto& Get() const
        {
            return std::get<N>(elems);
        }

        // 遍历元素列表，并对每个元素调用指定函数
        // 传入的函数Func只有一个参数，并且能接收元素列表中的每个元素
        template<typename Func>
        requires requires(Func func) { (func(std::declval<Elems>()) , ...); }
        constexpr void ForEach(const Func& func) const
        {
            ForEach<Func, 0>(func);
        }

        template<typename Func>
        requires requires(Func func) { (std::convertible_to<decltype(func(std::declval<Elems>())), bool>, ...); }
        constexpr std::size_t FindIf(Func func) const
        {
            return FindIf<Func, 0>(func);
        }

        // 根据名称查找元素，返回索引，若不存在则返回std::size_t最大值
        template<typename Name>
        constexpr std::size_t Find(const Name& name) const
        {
            return FindIf([&](const auto& field) { return field.name == name; });
        }

        // 根据值查找元素，返回索引，若不存在则返回std::size_t最大值
        template<typename T>
        constexpr std::size_t FindValue(const T& value) const
        {
            return FindIf([&value](const auto& field) { return field.value == value; });
        }

    private:
        template<typename Func, std::size_t N>
        constexpr void ForEach(const Func& func) const
        {
            if constexpr (N < Size)
            {
                func(Get<N>());
                ForEach<Func, N + 1>(func);
            }
        }

        template<typename Predicate, std::size_t N>
        constexpr std::size_t FindIf(const Predicate& func) const
        {
            if constexpr (N < Size)
            {
                if (func(Get<N>()))
                    return N;

                return FindIf<Predicate, N + 1>(func);
            }

            return std::numeric_limits<std::size_t>::max();
        }

        std::tuple<Elems...> elems;
    };
}