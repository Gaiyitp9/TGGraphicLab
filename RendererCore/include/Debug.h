#pragma once

#include "pch.h"

namespace IGGSZLab
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
		inline static void Log() {}
#else
		inline static void Log(const char* msg) { OutputDebugStringA(msg); }
		inline static void Log(const wchar_t* msg) { OutputDebugStringW(msg); }
#endif
	};

#ifdef ASSERT
#undef ASSERT
#endif

#ifdef RELEASE
#define ASSERT(isTrue, ...) (void)(isTrue)
#define ASSERT_SUCCEEDED(hr, ...) (void)(hr)
#else
#define STRINGIFY(x) #x
#define ASSERT(isTrue, ...)\
		if (!(bool)(isTrue))\
		{\
			Debug::Log("\nAssertion failed in " STRINGIFY(__FILE__) " @ " STRINGIFY(__LINE__) "\n");\
			Debug::Log("\'" #isTrue "\' is false");\
			Debug::Log(__VA_ARGS__);\
			Debug::Log("\n");\
			__debugbreak();\
		}

#define ASSERT_SUCCEEDED(hr, ...)\
		if (FAILED(hr))\
		{\
			Debug::Log("\nHRESULT failed in " STRINGIFY(__FILE__) " @ " STRINGIFY(__LINE__) "\n");\
			Debug::Log("hr = 0x%08X", hr);\
			Debug::Log(__VA_ARGS__);\
			Debug::Log("\n");\
			__debugbreak();\
		}
#endif
}