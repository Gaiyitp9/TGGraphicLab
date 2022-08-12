/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "WinAPIException.hpp"
#include <iostream>

#ifdef ThrowIfFailed
#undef ThrowIfFailed
#endif

#ifdef ThrowIfFailedWithDesc
#undef ThrowIfFailedWithDesc
#endif

#ifdef ThrowLastError
#undef ThrowLastError
#endif

#ifdef ThrowLastErrorWithDesc
#undef ThrowLastErrorWithDesc
#endif

#define ThrowBaseExcept(description)\
			throw LCH::BaseException(description);

#define ThrowIfFailed(hr)\
			if (FAILED(hr))\
			{\
				throw LCH::WinAPIException(hr);\
			}

#define ThrowIfFailedWithDesc(hr, description)\
			if (FAILED(hr))\
			{\
				throw LCH::WinAPIException(hr, description);\
			}

#define ThrowLastError()\
		{\
			DWORD hr = GetLastError();\
			if (hr > 0)\
			{\
				throw LCH::WinAPIException(hr);\
			}\
		}

#define ThrowLastErrorWithDesc(description)\
		{\
			DWORD hr = GetLastError();\
			if (hr > 0)\
			{\
				throw LCH::WinAPIException(hr, description);\
			}\
		}

namespace LCH
{
	template<typename Text> struct text_trait;
	template<typename Text> struct text_trait<const Text> : text_trait<Text> {};
	template<typename Text> struct text_trait<const Text&> : text_trait<Text> {};

	template<typename CharT>
	struct text_trait<std::basic_string<CharT>>
	{
		static constexpr bool wide = std::is_same_v<CharT, wchar_t>;
	};

	template<typename CharT>
	struct text_trait<CharT*>
	{
		static constexpr bool wide = std::is_same_v<CharT, wchar_t>;
	};

	template<typename CharT>
	struct text_trait<CharT[]>
	{
		static constexpr bool wide = std::is_same_v<CharT, wchar_t>;
	};
	
	class Debug
	{
		Debug() = delete;
		Debug(const Debug&) = delete;
		Debug& operator=(const Debug&) = delete;

	public:
		template<typename Text>
		static void Log(const Text& log)
		{
			if constexpr (text_trait<Text>::wide)
				std::wcout << log;
			else
				std::cout << log;
		}

		template<typename Text>
		static void LogLine(const Text& log)
		{
			if constexpr (text_trait<Text>::wide)
				std::wcout << log << std::endl;
			else
				std::cout << log << std::endl;
		}

		/*template <typename charT>
		static void Log(const std::basic_string<charT>& log)
		{
			if constexpr (std::is_same_v<charT, char>)
				std::cout << log;
			else
				std::wcout << log;
		}

		template <typename charT>
		static void LogLine(const std::basic_string<charT>& log)
		{
			if constexpr (std::is_same_v<charT, char>)
				std::cout << log << std::endl;
			else
				std::wcout << log << std::endl;
		}

		template <typename charT>
		static void Log(charT const* log)
		{
			if constexpr (std::is_same_v<charT, char>)
				std::cout << log;
			else
				std::wcout << log;
		}

		template <typename charT>
		static void LogLine(charT const* log)
		{
			if constexpr (std::is_same_v<charT, char>)
				std::cout << log << std::endl;
			else
				std::wcout << log << std::endl;
		}*/
	};
}