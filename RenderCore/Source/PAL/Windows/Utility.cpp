/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "PAL/Windows/Utility.h"
#include "PAL/Windows/Win32API.h"
#include <locale>

namespace TG::PAL::Utility
{
	std::wstring Utf8ToUtf16(std::string_view str)
	{
		const int length = MultiByteToWideChar(CP_UTF8, 0, str.data(), -1, nullptr, 0);
		auto* wide = static_cast<wchar_t*>(malloc(length * sizeof(wchar_t)));
		MultiByteToWideChar(CP_UTF8, 0, str.data(), -1, wide, length);
		std::wstring wstr(wide);
		free(wide);
		return wstr;
	}

	std::string Utf16ToUtf8(std::wstring_view wstr)
	{
		const int length = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), -1, nullptr, 0, nullptr, nullptr);
		auto* ansi = static_cast<char*>(malloc(length * sizeof(char)));
		WideCharToMultiByte(CP_UTF8, 0, wstr.data(), -1, ansi, length, nullptr, nullptr);
		std::string str(ansi);
		free(ansi);
		return str;
	}

	std::string ToLower(std::string_view str)
	{
		std::string lowerCase{str};
		const std::locale loc;
		for (char& s : lowerCase)
			s = std::tolower(s, loc);
		return lowerCase;
	}

	std::wstring ToLower(std::wstring_view wstr)
	{
		std::wstring lowerCase{wstr};
		const std::locale loc;
		for (wchar_t& s : lowerCase)
			s = std::tolower(s, loc);
		return lowerCase;
	}

	std::string GetBasePath(std::string_view filePath)
	{
		size_t lastSlash;
		if ((lastSlash = filePath.rfind('/')) != std::string::npos)
			return std::string{filePath.substr(0, lastSlash + 1)};
		if ((lastSlash = filePath.rfind('\\')) != std::string::npos)
			return std::string{filePath.substr(0, lastSlash + 1)};

        return {};
	}

	std::wstring GetBasePath(std::wstring_view filePath)
	{
		size_t lastSlash;
		if ((lastSlash = filePath.rfind(L'/')) != std::wstring::npos)
			return std::wstring{filePath.substr(0, lastSlash + 1)};
		if ((lastSlash = filePath.rfind(L'\\')) != std::wstring::npos)
			return std::wstring{filePath.substr(0, lastSlash + 1)};

		return {};
	}

	std::string RemoveBasePath(std::string_view filePath)
	{
		size_t lastSlash;
		if ((lastSlash = filePath.rfind('/')) != std::string::npos)
			return std::string{filePath.substr(lastSlash + 1)};
		if ((lastSlash = filePath.rfind('\\')) != std::string::npos)
			return std::string{filePath.substr(lastSlash + 1)};

		return std::string{filePath};
	}

	std::wstring RemoveBasePath(std::wstring_view filePath)
	{
		size_t lastSlash;
		if ((lastSlash = filePath.rfind(L'/')) != std::wstring::npos)
			return std::wstring{filePath.substr(lastSlash + 1)};
		if ((lastSlash = filePath.rfind(L'\\')) != std::wstring::npos)
			return std::wstring{filePath.substr(lastSlash + 1)};

		return std::wstring{filePath};
	}

	std::string GetFileExtension(std::string_view filePath)
	{
		const std::string fileName = RemoveBasePath(filePath);
		const size_t extOffset = fileName.rfind('.');
		if (extOffset == std::string::npos)
			return "";

		return fileName.substr(extOffset + 1);
	}

	std::wstring GetFileExtension(std::wstring_view filePath)
	{
		const std::wstring fileName = RemoveBasePath(filePath);
		const size_t extOffset = fileName.rfind(L'.');
		if (extOffset == std::wstring::npos)
			return L"";

		return fileName.substr(extOffset + 1);
	}

	inline std::string RemoveExtension(std::string_view filePath)
	{
		return std::string{filePath.substr(0, filePath.rfind('.'))};
	}

	inline std::wstring RemoveExtension(std::wstring_view filePath)
	{
		return std::wstring{filePath.substr(0, filePath.rfind(L'.'))};
	}
}
