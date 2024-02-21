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
    class ConstString
    {
        constexpr static std::size_t Size = N;
        constexpr static std::size_t NPos = static_cast<std::size_t>(-1);

    public:
        constexpr ConstString()
            : m_data{}
        {}
        constexpr explicit ConstString(const CharT(&str)[N + 1])
            : ConstString(str, std::make_index_sequence<N>())
        {}
        constexpr ConstString(const ConstString& other)
            : ConstString(other, std::make_index_sequence<N>())
        {}

        constexpr CharT operator[](std::size_t index) const
        {
            return m_data[index];
        }

        constexpr std::basic_string_view<CharT> View() const noexcept
        {
            return m_data;
        }

        template<std::size_t Pos, std::size_t Count = NPos> requires (Pos <= N)
        constexpr auto Substr() const
        {
            constexpr std::size_t length = std::min(Count, N - Pos);

            char data[length + 1]{};
            for (std::size_t i = 0; i < length; i++)
                data[i] = m_data[Pos + i];

            return ConstString<CharT, length>(data);
        }

    private:
        template<std::size_t... Idx>
        constexpr ConstString(const CharT(&str)[sizeof...(Idx) + 1], std::index_sequence<Idx...>)
            : m_data{str[Idx]..., 0}
        {}

        template<std::size_t... Idx>
        constexpr ConstString(const ConstString& other, std::index_sequence<Idx...>)
            : m_data{other.m_data[Idx]..., 0}
        {}

        CharT m_data[N + 1];
    };

    // 编译期字符串推导指南(deduction guide)
    template<typename CharT, std::size_t N>
    ConstString(const CharT(&)[N])->ConstString<CharT, N - 1>;

    ConstString()->ConstString<char, 0>;

    // 不能定义在ConstexprString内部，因为如果==两边字符串的CharT或者N不同，编译器无法确定使用哪一边的字符串推导CharT和N，导致无法推导模板参数
    template<typename CharT, std::size_t M, std::size_t N>
    constexpr bool operator==(const ConstString<CharT, M>& lhs, const ConstString<CharT, N>& rhs)
    {
        if constexpr (N != M)
            return false;
        else
            return lhs.View() == rhs.View();
    }

    template<typename CharT, std::size_t M, std::size_t N>
    constexpr ConstString<CharT, M + N> operator+(const ConstString<CharT, M>& lhs, const ConstString<CharT, N>& rhs)
    {
        CharT data[M + N + 1]{};
        for (int i = 0; i < M; ++i)
            data[i] = lhs[i];
        for (int i = 0; i < N; ++i)
            data[M + i] = rhs[i];
        return data;
    }

    template<typename CharT, std::size_t M, std::size_t N>
    constexpr ConstString<CharT, M + N - 1> operator+(const ConstString<CharT, M>& lhs, const CharT(&rhs)[N])
    {
        return lhs + ConstString(rhs);
    }

    template<typename CharT, std::size_t M, std::size_t N>
    constexpr ConstString<CharT, M + N - 1> operator+(const CharT(&lhs)[M], const ConstString<CharT, N>& rhs)
    {
        return ConstString(lhs) + rhs;
    }
}