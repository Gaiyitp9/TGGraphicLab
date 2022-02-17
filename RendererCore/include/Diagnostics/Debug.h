/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <comdef.h>

namespace LCH
{
	class Debug
	{
	public:
		inline static void LogFormat(const char* format, ...)
		{
			char buffer[256];
			va_list ap;
			va_start(ap, format);
			vsprintf_s(buffer, 256, format, ap);
			va_end(ap);
			Log(buffer);
		}

		inline static void LogFormat(const wchar_t* format, ...)
		{
			wchar_t buffer[256];
			va_list ap;
			va_start(ap, format);
			vswprintf(buffer, 256, format, ap);
			va_end(ap);
			Log(buffer);
		}

#ifdef _CONSOLE
		inline static void Log(const char* msg) { printf("%s", msg); }
		inline static void Log(const wchar_t* msg) { wprintf(L"%ws", msg); }
#else
		inline static void Log(const char* msg) { OutputDebugStringA(msg); }
		inline static void Log(const wchar_t* msg) { OutputDebugStringW(msg); }
#endif
	};

#ifdef ASSERT
#undef ASSERT
#endif

#ifdef NDEBUG
#define ASSERT(isTrue, checkHrError) isTrue
#define ASSERT_SUCCEEDED(hr) hr
#else
#define STRINGIFY(x) #x
// checkHrError表示是否要检查错误码(因为有些函数不返回错误码，需要用GetLastError来查询)
#define ASSERT(isTrue, checkHrError)\
		if (!(isTrue))\
		{\
			std::string errInfo = "\nAssertion failed in " __FILE__ ": ";\
			errInfo += std::to_string(__LINE__);\
			errInfo += "\n\'" #isTrue "\' is false.\n";\
			Debug::Log(errInfo.c_str());\
			if (checkHrError)\
			{\
				_com_error err(GetLastError());\
				Debug::Log(err.ErrorMessage());\
			}\
			__debugbreak();\
		}

#define ASSERT_SUCCEEDED(hr)\
		if (FAILED(hr))\
		{\
			std::string errInfo = "\nHRESULT failed in " __FILE__ ": ";\
			errInfo += std::to_string(__LINE__);\
			Debug::Log(errInfo.c_str());\
			Debug::LogFormat("\nhr = 0x%08X\n", hr);\
			_com_error err(hr);\
			Debug::Log(err.ErrorMessage());\
			__debugbreak();\
		}
#endif
}