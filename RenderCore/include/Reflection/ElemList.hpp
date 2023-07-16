/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Reflection
{
    template<typename... Elems>
    struct ElemList
    {
        static constexpr std::size_t Size = sizeof...(Elems);

        constexpr explicit ElemList(Elems... elems) : elems{elems...} {}

        // 遍历元素列表，并对每个元素调用指定函数
        // 传入的函数Func只有一个参数，并且能接收元素列表中的每个元素
        template<typename Func>
        requires requires(Func func) { (func(std::declval<Elems>()) , ...); }
        constexpr void ForEach(const Func& func) const
        {
            if constexpr (Size > 0)
                ForEach(func, std::make_index_sequence<Size>());
        }

        // 根据名称寻找元素
        template<typename Name>
        constexpr const auto& Find(Name) const
        {

        }

        // 判断是否包含指定名称的元素
        // 如果要调用该函数，需要保证所有元素都有NameIs静态函数
        template<typename Name>
        requires requires { (Elems::template NameIs<Name>() , ...); }
        constexpr bool Contains(Name) const
        {
            return (... || Elems::template NameIs<Name>());
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
            if constexpr (sizeof...(Ns) > 0)
            {
                func(Get<N0>());
                ForEach(func, std::index_sequence<Ns...>());
            }
        }

        std::tuple<Elems...> elems;
    };
}