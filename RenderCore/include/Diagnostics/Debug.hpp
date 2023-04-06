/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <iostream>

namespace TG
{
	template<typename Text> struct TextTrait;
	template<typename Text> struct TextTrait<const Text> : TextTrait<Text> {};

	template<typename CharT> struct TextTrait<CharT*>
	{
        [[maybe_unused]] static constexpr bool wideStream = std::is_same_v<CharT, wchar_t>;
	};
	// charT const*必须分开写，否则识别不了wchar_t，暂时不知道原因。下同
	template<typename CharT> struct TextTrait<CharT const*> : TextTrait<CharT*> {};
	template<typename CharT, size_t N> struct TextTrait<CharT[N]> : TextTrait<CharT*> {};
	template<typename CharT, size_t N> struct TextTrait<const CharT[N]> : TextTrait<CharT*> {};
    // It's a reference to array of const char of size N.
    // This is probably used to accept a string literal as argument.
    // https://stackoverflow.com/questions/43435279/what-does-const-charan-mean
	template<typename CharT, size_t N> struct TextTrait<CharT(&)[N]> : TextTrait<CharT*> {};
	template<typename CharT, size_t N> struct TextTrait<const CharT(&)[N]> : TextTrait<CharT*> {};
	template<typename CharT> struct TextTrait<std::basic_string<CharT>> : TextTrait<CharT*> {};

	class Debug
	{
    public:
		Debug() = delete;
		Debug(const Debug&) = delete;
		Debug& operator=(const Debug&) = delete;

	public:
		template<typename Text>
		static void Log(const Text& log) { std::cout << log; }
		template<typename Text> requires TextTrait<Text>::wideStream
		static void Log(const Text& log) { std::wcout << log; }
		template<typename Text>
		static void LogLine(const Text& log) { std::cout << log << std::endl; }
		template<typename Text> requires TextTrait<Text>::wideStream
		static void LogLine(const Text& log) { std::wcout << log << std::endl; }
	};
}