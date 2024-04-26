/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <iostream>

namespace TG::Debug
{
	template<typename CharT>
	struct TextTraits
    {
        static constexpr bool IsText = std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>;
        static constexpr bool Wide = std::is_same_v<CharT, wchar_t>;
    };
    // 目前支持三种文本类型：字符串，std::string和std::string_view
	template<typename CharT> struct TextTraits<CharT*> : TextTraits<CharT> {};
    // std::decay不能把const char*转换成char*
	template<typename CharT> struct TextTraits<const CharT*> : TextTraits<CharT> {};
	template<typename CharT> struct TextTraits<std::basic_string<CharT>> : TextTraits<CharT> {};
    template<typename CharT> struct TextTraits<std::basic_string_view<CharT>> : TextTraits<CharT> {};

    // 窄文本概念，文本包含的字符是char类型
    template<typename Text>
    concept NarrowText = TextTraits<std::decay_t<Text>>::IsText && !TextTraits<std::decay_t<Text>>::Wide;
    // 宽文本概念，文本包含的是wchar_t类型
    template<typename Text>
    concept WideText = TextTraits<std::decay_t<Text>>::IsText && TextTraits<std::decay_t<Text>>::Wide;

    // 注：1. 尽量使用窄文本，性能比宽文本好
    // 2. 换行符用\n，不要用std::endl，后者会刷新缓冲区到设备，会影响性能
    template<NarrowText Text>
    void Log(const Text&log) { std::cout << log; }

    template<WideText Text>
    void Log(const Text&log) { std::wcout << log; }

    template<NarrowText Text>
    void LogLine(const Text&log) { std::cout << log << '\n'; }

    template<WideText Text>
    void LogLine(const Text&log) { std::wcout << log << L'\n'; }
}
