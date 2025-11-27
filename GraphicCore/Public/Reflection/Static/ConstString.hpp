/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Reflection
{
    // 编译期字符串 compile-time string
    template<std::size_t N>
    class ConstString
    {
    public:
        static constexpr std::size_t Size = N;
        static constexpr std::size_t NPos = static_cast<std::size_t>(-1);

        constexpr ConstString() noexcept : m_data{}
        {}
        constexpr explicit ConstString(const char(&str)[N + 1])
            : ConstString(str, std::make_index_sequence<N>{})
        {}
        constexpr ConstString(const ConstString& other)
            : ConstString(other, std::make_index_sequence<N>{})
        {}

        static constexpr ConstString CreateFromCString(const char* const str)
        {
            ConstString result;
            for (size_t i = 0; i < N; i++)
                result.m_data[i] = str[i];

            return result;
        }

        constexpr explicit operator const char*() const noexcept
        {
            return m_data;
        }

        [[nodiscard]] constexpr const char* CStr() const noexcept
        {
            return m_data;
        }

        constexpr explicit operator std::string_view() const noexcept
        {
            return m_data;
        }

        [[nodiscard]] constexpr std::string_view View() const noexcept
        {
            return m_data;
        }

        constexpr char operator[](std::size_t index) const requires (index <= N)
        {
            return m_data[index];
        }

        constexpr char& operator[](std::size_t index) requires (index <= N)
        {
            return m_data[index];
        }

        template<std::size_t Pos, std::size_t Count = NPos> requires (Pos <= N)
        constexpr auto Substr() const
        {
            constexpr std::size_t length = std::min(Count, N - Pos);

            char data[length + 1]{};
            for (std::size_t i = 0; i < length; i++)
                data[i] = m_data[Pos + i];

            return ConstString<length>(data);
        }

        constexpr auto Find(char ch, std::size_t pos = 0) const
        {
            for (std::size_t i = pos; i < N; ++i)
            {
                if (m_data[i] == ch)
                    return i;
            }
            return NPos;
        }

        constexpr auto RFind(char ch, std::size_t pos = NPos) const
        {
            for (std::size_t i = (pos == NPos ? N - 1 : pos); i + 1 > 0; --i)
            {
                if (m_data[i] == ch)
                    return i;
            }
            return NPos;
        }

    private:
        template<std::size_t... Indices>
        constexpr ConstString(const char(&str)[sizeof...(Indices) + 1], std::index_sequence<Indices...>)
            : m_data{ str[Indices]..., 0 }
        {}

        template<std::size_t... Indices>
        constexpr ConstString(const ConstString& other, std::index_sequence<Indices...>)
            : m_data{ other.m_data[Indices]..., 0 }
        {}

        char m_data[N + 1];
    };

    // 编译期字符串推导指南(deduction guide)
    template<std::size_t N>
    ConstString(const char(&)[N])->ConstString<N - 1>;

    ConstString()->ConstString<0>;

    // 不能定义在ConstString内部，因为如果==两边字符串的CharT或者N不同，编译器无法确定使用哪一边的字符串
    // 来调用operator==推导CharT和N，导致无法推导模板参数
    template<std::size_t M, std::size_t N>
    constexpr bool operator==(const ConstString<M>& lhs, const ConstString<N>& rhs)
    {
        if constexpr (N != M)
            return false;
        else
            return lhs.View() == rhs.View();
    }

    template<std::size_t M, std::size_t N>
    constexpr bool operator!=(const ConstString<M>& lhs, const ConstString<N>& rhs)
    {
        return !(lhs == rhs);
    }

    template<std::size_t M, std::size_t N>
    constexpr bool operator==(const ConstString<M>& lhs, const char(&rhs)[N])
    {
        return lhs == ConstString(rhs);
    }

    template<std::size_t M, std::size_t N>
    constexpr bool operator!=(const ConstString<M>& lhs, const char(&rhs)[N])
    {
        return lhs != ConstString(rhs);
    }

    template<std::size_t M, std::size_t N>
    constexpr bool operator==(const char(&lhs)[M], const ConstString<N>& rhs)
    {
        return ConstString(lhs) == rhs;
    }

    template<std::size_t M, std::size_t N>
    constexpr bool operator!=(const char(&lhs)[M], const ConstString<N>& rhs)
    {
        return ConstString(lhs) != rhs;
    }

    template<std::size_t M, std::size_t N>
    constexpr ConstString<M + N> operator+(const ConstString<M>& lhs, const ConstString<N>& rhs)
    {
        char data[M + N + 1]{};
        for (int i = 0; i < M; ++i)
            data[i] = lhs[i];
        for (int i = 0; i < N; ++i)
            data[M + i] = rhs[i];
        return data;
    }

    template<std::size_t M, std::size_t N>
    constexpr ConstString<M + N - 1> operator+(const ConstString<M>& lhs, const char(&rhs)[N])
    {
        return lhs + ConstString(rhs);
    }

    template<std::size_t M, std::size_t N>
    constexpr ConstString<M + N - 1> operator+(const char(&lhs)[M], const ConstString<N>& rhs)
    {
        return ConstString(lhs) + rhs;
    }

    /**
     * \brief 使用const char*创建ConstString
     * \param CSTRING c风格字符串
     */
#define CREATE_CONST_STRING(CSTRING) \
    ConstString<std::char_traits<char>::length(CSTRING)>::CreateFromCString(CSTRING);
}
