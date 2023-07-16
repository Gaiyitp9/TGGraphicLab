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
    template<typename CharT, std::size_t N>
    struct ConstexprString
    {
        using CharType = CharT;
        static constexpr std::size_t Size = N;

        constexpr explicit ConstexprString(const CharType(&str)[N + 1]) noexcept
            : ConstexprString(str, std::make_index_sequence<N>())
        {}

        CharType data[N + 1];

    private:
        template<std::size_t... Idx>
        constexpr explicit ConstexprString(const CharType *str, std::index_sequence<Idx...>) noexcept
            : data{ str[Idx]..., 0}
        {}
    };

    template<typename CharT, std::size_t N>
    ConstexprString(const CharT(&)[N])->ConstexprString<CharT, N - 1>;

    template<ConstexprString str>
    struct TStr
    {
        using CharType = typename decltype(str)::CharType;
    };
}