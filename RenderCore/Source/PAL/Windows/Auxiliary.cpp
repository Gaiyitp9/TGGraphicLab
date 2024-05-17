/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "PAL/Windows/Auxiliary.h"
#include "PAL/Windows/Win32API.h"

namespace TG::PAL
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
}
