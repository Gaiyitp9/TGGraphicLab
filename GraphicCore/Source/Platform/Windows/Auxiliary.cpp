/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Platform/Windows/Auxiliary.h"
#include "Platform/Core.h"
#include <chrono>

namespace TG::Platform
{
	std::wstring Utf8ToUtf16(std::string_view str)
	{
		int length = MultiByteToWideChar(CP_UTF8, 0, str.data(), -1, nullptr, 0);
		auto* wide = static_cast<wchar_t*>(malloc(length * sizeof(wchar_t)));
		MultiByteToWideChar(CP_UTF8, 0, str.data(), -1, wide, length);
		std::wstring wstr(wide);
		free(wide);
		return wstr;
	}

	std::string Utf16ToUtf8(std::wstring_view wstr)
	{
		int length = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), -1, nullptr, 0, nullptr, nullptr);
		auto* ansi = static_cast<char*>(malloc(length * sizeof(char)));
		WideCharToMultiByte(CP_UTF8, 0, wstr.data(), -1, ansi, length, nullptr, nullptr);
		std::string str(ansi);
		free(ansi);
		return str;
	}

	// 在Windows平台上调用std::chrono::current_zone()后会产生内存泄漏
	// https://developercommunity.visualstudio.com/t/std::chrono::current_zone-produces-a/1513362
	class ChronoMemoryLeakHelper
	{
	public:
		ChronoMemoryLeakHelper() { std::ignore = std::chrono::current_zone(); }
		// 解决chrono中current_zone在Windows平台上内存泄漏问题 https://github.com/microsoft/STL/issues/2504
		// 注意，不推荐直接调用析构函数。这里用这种方法移除这个内存泄漏报告，因为在Windows平台上开启CRT memory checking时
		// tzdb会泄漏内存，具体原因看帖子，
		~ChronoMemoryLeakHelper() { std::chrono::get_tzdb_list().~tzdb_list(); }
	};
	static ChronoMemoryLeakHelper gIgnore;
}
