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
	class Debug
	{
	public:
		Debug() = delete;
		Debug(const Debug&) = delete;
		Debug& operator=(const Debug&) = delete;

	public:
		template <typename charT>
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
		}
	};
}