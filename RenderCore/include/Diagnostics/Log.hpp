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
	struct TextTraits
    {
        using TextType = Text;
        static constexpr bool IsText = std::is_same_v<TextType, char> || std::is_same_v<TextType, wchar_t>;
        static constexpr bool Wide = std::is_same_v<TextType, wchar_t>;
    };
    template<typename Text> struct TextTraits<const Text> : TextTraits<Text> {};    // 移除Text的const修饰符
	template<typename CharT> struct TextTraits<CharT*> : TextTraits<CharT> {};
	template<typename CharT, size_t N> struct TextTraits<CharT[N]> : TextTraits<CharT> {};
	template<typename CharT> struct TextTraits<std::basic_string<CharT>> : TextTraits<CharT> {};
    template<typename CharT> struct TextTraits<std::basic_string_view<CharT>> : TextTraits<CharT> {};

    // 窄文本概念，文本包含的字符是char类型
    template<typename Text>
    concept NarrowText = TextTraits<Text>::IsText && !TextTraits<Text>::Wide;
    // 宽文本概念，文本包含的是wchar_t类型
    template<typename Text>
    concept WideText = TextTraits<Text>::IsText && TextTraits<Text>::Wide;

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