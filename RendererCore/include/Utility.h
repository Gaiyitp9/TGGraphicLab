//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
// Developed by Minigraph
//
// Author:  James Stanard 
//

#pragma once

#include "pch.h"

namespace Utility
{
#ifdef _CONSOLE
	inline void Print(const char* msg) { printf("%s", msg); }
	inline void Print(const wchar_t* msg) { wprintf(L"%ws", msg); }
#else
	inline void Print(const char* msg) { OutputDebugStringA(msg); }
	inline void Print(const wchar_t* msg) { OutputDebugStringW(msg); }
#endif

	inline void Printf(const char* format, ...)
	{
		char buffer[256];
		va_list ap;
		va_start(ap, format);
		vsprintf_s(buffer, 256, format, ap);
		va_end(ap);
		Print(buffer);
	}

	inline void Printf(const wchar_t* format, ...)
	{
		wchar_t buffer[256];
		va_list ap;
		va_start(ap, format);
		vswprintf(buffer, 256, format, ap);
		va_end(ap);
		Print(buffer);
	}

#ifndef RELEASE
	inline void PrintSubMessage(const char* format, ...)
	{
		Print("--> ");
		char buffer[256];
		va_list ap;
		va_start(ap, format);
		vsprintf_s(buffer, 256, format, ap);
		va_end(ap);
		Print(buffer);
		Print("\n");
	}
	inline void PrintSubMessage(const wchar_t* format, ...)
	{
		Print("--> ");
		wchar_t buffer[256];
		va_list ap;
		va_start(ap, format);
		vswprintf(buffer, 256, format, ap);
		va_end(ap);
		Print(buffer);
		Print("\n");
	}
#endif

	std::wstring AnsiToWideString(const std::string& str);
	std::string WideStringToAnsi(const std::wstring& wstr);
	std::string ToLower(const std::string& str);
	std::wstring ToLower(const std::wstring& wstr);
	std::string GetBasePath(const std::string& str);
	std::wstring GetBasePath(const std::wstring& wstr);
	std::string RemoveBasePath(const std::string& str);
	std::wstring RemoveBasePath(const std::wstring& wstr);
	std::string GetFileExtension(const std::string& str);
	std::wstring GetFileExtension(const std::wstring& wstr);
	std::string RemoveExtension(const std::string& str);
	std::wstring RemoveExtension(const std::wstring& wstr);
}

#ifdef ERROR
#undef ERROR
#endif
#ifdef ASSERT
#undef ASSERT
#endif
#ifdef HALT
#undef HALT
#endif

#define HALT(...) ERROR(__VA_ARGS__) __debugbreak();

#ifdef RELEASE
	#define ASSERT(isTrue, ...) (void)(isTrue)
	#define ASSERT_SUCCEEDED(hr, ...) (void)(hr)
	#define WARN_ONCE_IF(isTrue, ...) (void)(isTrue)
	#define WARN_ONCE_IF_NOT(isTrue, ...) (void)(isTrue)
	#define ERROR(msg, ...)
	#define DEBUGPRINT(msg, ...)
#else
	#define STRINGIFY(x) #x
	#define STRINGIFY_BUILTIN(x) STRINGIFY(x)
	#define ASSERT(isTrue, ...)\
		if (!(bool)(isTrue))\
		{\
			Utility::Print("\nAssertion failed in " STRINGIFY_BUILTIN(__FILE__) " @ " STRINGIFY_BUILTIN(__LINE__) "\n");\
			Utility::PrintSubMessage("\'" #isTrue "\' is false");\
			Utility::PrintSubMessage(__VA_ARGS__);\
			Utility::Print("\n");\
			__debugbreak();\
		}

	#define ASSERT_SUCCEEDED(hr, ...)\
		if (FAILED(hr))\
		{\
			Utility::Print("\nHRESULT failed in " STRINGIFY_BUILTIN(__FILE__) " @ " STRINGIFY_BUILTIN(__LINE__) "\n");\
			Utility::PrintSubMessage("hr = 0x%08X", hr);\
			Utility::PrintSubMessage(__VA_ARGS__);\
			Utility::Print("\n");\
			__debugbreak();\
		}

	#define WARN_ONCE_IF(isTrue, ...)\
		static bool s_TriggeredWarning = false;\
		if ((bool)(isTrue) && !s_TriggeredWarning)\
		{\
			s_TriggeredWarning = true;\
			Utility::Print("\nWarning issued in " STRINGIFY_BUILTIN(__FILE__) " @ " STRINGIFY_BUILTIN(__LINE__) "\n");\
			Utility::PrintSubMessage("\'" #isTrue "\' is true");\
			Utility::PrintSubMessage(__VA_ARGS__);\
			Utility::Print("\n");\
		}

	#define WARN_ONCE_IF_NOT(isTrue, ...) WARN_ONCE_IF(!(isTrue), __VA_ARGS__)

	#define ERROR(...)\
		Utility::Print("\nError reported in " STRINGIFY_BUILTIN(__FILE__) " @ " STRINGIFY_BUILTIN(__LINE__) "\n");\
		Utility::PrintSubMessage(__VA_ARGS__);\
		Utility::Print("\n");

	#define DEBUGPRINT(msg, ...)\
	Utility::Printf(msg "\n", ##__VA_ARGS__);
#endif

#define BreakIfFailed(hr) if (FAILED(hr)) __debugbreak()

void SIMDMemCopy(void* __restrict dest, const void* __restrict source, size_t numQuadwords);
void SIMDMemFill(void* __restrict dest, __m128 fillVector, size_t numQuadwords);