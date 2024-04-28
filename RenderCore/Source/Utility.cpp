/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Utility.h"
#include "Win32Lean.h"
#include <locale>

namespace TG::Utility
{
	std::wstring Utf8ToUtf16(const std::string& str)
	{
		const int length = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
		auto* wide = static_cast<wchar_t*>(malloc(length * sizeof(wchar_t)));
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wide, length);
		std::wstring wstr(wide);
		free(wide);
		return wstr;
	}

	std::string Utf16ToUtf8(const std::wstring& wstr)
	{
		const int length = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
		auto* ansi = static_cast<char*>(malloc(length * sizeof(char)));
		WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, ansi, length, nullptr, nullptr);
		std::string str(ansi);
		free(ansi);
		return str;
	}

	std::string ToLower(const std::string& str)
	{
		std::string lowerCase = str;
		const std::locale loc;
		for (char& s : lowerCase)
			s = std::tolower(s, loc);
		return lowerCase;
	}

	std::wstring ToLower(const std::wstring& wstr)
	{
		std::wstring lowerCase = wstr;
		const std::locale loc;
		for (wchar_t& s : lowerCase)
			s = std::tolower(s, loc);
		return lowerCase;
	}

	std::string GetBasePath(const std::string& filePath)
	{
		size_t lastSlash;
		if ((lastSlash = filePath.rfind('/')) != std::string::npos)
			return filePath.substr(0, lastSlash + 1);
		if ((lastSlash = filePath.rfind('\\')) != std::string::npos)
			return filePath.substr(0, lastSlash + 1);

        return "";
	}

	std::wstring GetBasePath(const std::wstring& filePath)
	{
		size_t lastSlash;
		if ((lastSlash = filePath.rfind(L'/')) != std::wstring::npos)
			return filePath.substr(0, lastSlash + 1);
		if ((lastSlash = filePath.rfind(L'\\')) != std::wstring::npos)
			return filePath.substr(0, lastSlash + 1);

		return L"";
	}

	std::string RemoveBasePath(const std::string& filePath)
	{
		size_t lastSlash;
		if ((lastSlash = filePath.rfind('/')) != std::string::npos)
			return filePath.substr(lastSlash + 1);
		if ((lastSlash = filePath.rfind('\\')) != std::string::npos)
			return filePath.substr(lastSlash + 1);

		return filePath;
	}

	std::wstring RemoveBasePath(const std::wstring& filePath)
	{
		size_t lastSlash;
		if ((lastSlash = filePath.rfind(L'/')) != std::wstring::npos)
			return filePath.substr(lastSlash + 1);
		if ((lastSlash = filePath.rfind(L'\\')) != std::wstring::npos)
			return filePath.substr(lastSlash + 1);

		return filePath;
	}

	std::string GetFileExtension(const std::string& filePath)
	{
		const std::string fileName = RemoveBasePath(filePath);
		const size_t extOffset = fileName.rfind('.');
		if (extOffset == std::string::npos)
			return "";

		return fileName.substr(extOffset + 1);
	}

	std::wstring GetFileExtension(const std::wstring& filePath)
	{
		const std::wstring fileName = RemoveBasePath(filePath);
		const size_t extOffset = fileName.rfind(L'.');
		if (extOffset == std::wstring::npos)
			return L"";

		return fileName.substr(extOffset + 1);
	}

	std::string RemoveExtension(const std::string& filePath)
	{
		return filePath.substr(0, filePath.rfind('.'));
	}

	std::wstring RemoveExtension(const std::wstring& filePath)
	{
		return filePath.substr(0, filePath.rfind(L'.'));
	}
}
