/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <iostream>

namespace TG::Debug
{
	template<typename CharT>
	struct TextTrait
    {
        using CharType = CharT;
        static constexpr bool Wide = std::is_same_v<CharT, wchar_t>;
    };
	template<typename CharT> struct TextTrait<const CharT> : TextTrait<CharT> {};

	// charT const*必须分开写，否则识别不了wchar_t，暂时不知道原因。下同
	template<typename CharT> struct TextTrait<CharT*> : TextTrait<CharT> {};
	template<typename CharT> struct TextTrait<CharT const*> : TextTrait<CharT> {};
	template<typename CharT, size_t N> struct TextTrait<CharT[N]> : TextTrait<CharT> {};
	template<typename CharT, size_t N> struct TextTrait<const CharT[N]> : TextTrait<CharT> {};
    // 字符串常量 https://stackoverflow.com/questions/43435279/what-does-const-charan-mean
	template<typename CharT, size_t N> struct TextTrait<CharT(&)[N]> : TextTrait<CharT> {};
	template<typename CharT, size_t N> struct TextTrait<const CharT(&)[N]> : TextTrait<CharT> {};

	template<typename CharT> struct TextTrait<std::basic_string<CharT>> : TextTrait<CharT> {};

    // 字符概念
    template<typename T>
    concept Character = std::is_same_v<T, char> || std::is_same_v<T, wchar_t>;
    // 窄文本概念，文本包含的字符是char类型
    template<typename Text>
    concept NarrowText = requires(Text t)
    {
        requires Character<typename TextTrait<Text>::CharType>;
        requires !TextTrait<Text>::Wide;
    };
    // 宽文本概念，文本包含的是wchar_t类型
    template<typename Text>
    concept WideText = requires
    {
        requires Character<typename TextTrait<Text>::CharType>;
        requires TextTrait<Text>::Wide;
    };

    template<NarrowText Text>
    inline void Log(const Text &log) { std::cout << log; }

    template<WideText Text>
    inline void Log(const Text &log) { std::wcout << log; }

    template<NarrowText Text>
    inline void LogLine(const Text &log) { std::cout << log << std::endl; }

    template<WideText Text>
    inline void LogLine(const Text &log) { std::wcout << log << std::endl; }
}