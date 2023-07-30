/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <string>

namespace TG
{
    // 编译期字符串 compile-time string
    template<typename Char, std::size_t N>
    struct ConstexprString
    {
        using CharT = Char;
        constexpr static std::size_t Size = N;
        constexpr static std::size_t NPos = static_cast<std::size_t>(-1);

        constexpr ConstexprString() noexcept
            : data{}
        {}
        constexpr explicit ConstexprString(const CharT(&str)[N + 1]) noexcept
            : ConstexprString(str, std::make_index_sequence<N>())
        {}
        constexpr ConstexprString(const ConstexprString& other) noexcept
            : ConstexprString(other, std::make_index_sequence<N>())
        {}

        constexpr std::basic_string_view<CharT> View() const
        {
            return data;
        }

    private:
        template<std::size_t... Idx>
        constexpr ConstexprString(const char(&str)[sizeof...(Idx) + 1], std::index_sequence<Idx...>) noexcept
            : data{ str[Idx]..., 0}
        {}

        template<std::size_t... Idx>
        constexpr ConstexprString(const ConstexprString& other, std::index_sequence<Idx...>) noexcept
            : data{ other.data[Idx]..., 0}
        {}

        CharT data[N + 1];
    };

    template<typename CharT, std::size_t N>
    ConstexprString(const CharT(&)[N])->ConstexprString<CharT, N - 1>;

    template<typename CharL, std::size_t M, typename CharR, std::size_t N>
    constexpr bool operator==(const ConstexprString<CharL, M>& lhs, const ConstexprString<CharR, N>& rhs)
    {
        if constexpr (!std::is_same_v<CharL, CharR> || N != M)
            return false;
        else
            return lhs.View() == rhs.View();
    }
}