/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <iostream>

namespace TG::Debug
{
	template<typename Text>
	struct TextTrait
    {
        using CharType = std::remove_cvref_t<Text>;
        static constexpr bool Wide = std::is_same_v<CharType, wchar_t>;
    };

	template<typename CharT> struct TextTrait<CharT*> : TextTrait<CharT> {};
	template<typename CharT, size_t N> struct TextTrait<CharT[N]> : TextTrait<CharT> {};
    // 字符串常量 https://stackoverflow.com/questions/43435279/what-does-const-charan-mean
	template<typename CharT, size_t N> struct TextTrait<CharT(&)[N]> : TextTrait<CharT> {};
	template<typename CharT> struct TextTrait<std::basic_string<CharT>> : TextTrait<CharT> {};
    template<typename CharT> struct TextTrait<std::basic_string_view<CharT>> : TextTrait<CharT> {};

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

    // 注：1. 尽量使用窄文本，性能比宽文本好
    // 2. 换行符用\n，不要用std::endl，后者会刷新缓冲区到设备，会影响性能
    template<NarrowText Text>
    inline void Log(const Text &log) { std::cout << log; }

    template<WideText Text>
    inline void Log(const Text &log) { std::wcout << log; }

    template<NarrowText Text>
    inline void LogLine(const Text &log) { std::cout << log << '\n'; }

    template<WideText Text>
    inline void LogLine(const Text &log) { std::wcout << log << L'\n'; }
}