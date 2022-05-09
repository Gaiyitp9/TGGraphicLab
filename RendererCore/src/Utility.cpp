/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Diagnostics/MemoryDbg.h"		// 必须放在文件头部，保证malloc使用的是debug版本
#include "Utility.h"

namespace LCH
{
	std::wstring Utility::AnsiToWideString(const std::string& str)
	{
		int length = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
		wchar_t* wide = new wchar_t[length];
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wide, length);
		std::wstring wstr(wide);
		delete[] wide;
		return wstr;
	}

	std::string Utility::WideStringToAnsi(const std::wstring& wstr)
	{
		int length = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
		char* ansi = new char[length];
		WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, ansi, length, nullptr, nullptr);
		std::string str(ansi);
		delete[] ansi;
		return str;
	}

	std::string Utility::ToLower(const std::string& str)
	{
		std::string lower_case = str;
		std::locale loc;
		for (char& s : lower_case)
			s = std::tolower(s, loc);
		return lower_case;
	}

	std::wstring Utility::ToLower(const std::wstring& wstr)
	{
		std::wstring lower_case = wstr;
		std::locale loc;
		for (wchar_t& s : lower_case)
			s = std::tolower(s, loc);
		return lower_case;
	}

	std::string Utility::GetBasePath(const std::string& filePath)
	{
		size_t lastSlash;
		if ((lastSlash = filePath.rfind('/')) != std::string::npos)
			return filePath.substr(0, lastSlash + 1);
		else if ((lastSlash = filePath.rfind('\\')) != std::string::npos)
			return filePath.substr(0, lastSlash + 1);
		else
			return "";
	}

	std::wstring Utility::GetBasePath(const std::wstring& filePath)
	{
		size_t lastSlash;
		if ((lastSlash = filePath.rfind(L'/')) != std::wstring::npos)
			return filePath.substr(0, lastSlash + 1);
		else if ((lastSlash = filePath.rfind(L'\\')) != std::wstring::npos)
			return filePath.substr(0, lastSlash + 1);
		else
			return L"";
	}

	std::string Utility::RemoveBasePath(const std::string& filePath)
	{
		size_t lastSlash;
		if ((lastSlash = filePath.rfind('/')) != std::string::npos)
			return filePath.substr(lastSlash + 1);
		else if ((lastSlash = filePath.rfind('\\')) != std::string::npos)
			return filePath.substr(lastSlash + 1);
		else
			return filePath;
	}

	std::wstring Utility::RemoveBasePath(const std::wstring& filePath)
	{
		size_t lastSlash;
		if ((lastSlash = filePath.rfind(L'/')) != std::wstring::npos)
			return filePath.substr(lastSlash + 1);
		else if ((lastSlash = filePath.rfind(L'\\')) != std::wstring::npos)
			return filePath.substr(lastSlash + 1);
		else
			return filePath;
	}

	std::string Utility::GetFileExtension(const std::string& filePath)
	{
		std::string fileName = RemoveBasePath(filePath);
		size_t extOffset = fileName.rfind('.');
		if (extOffset == std::string::npos)
			return "";

		return fileName.substr(extOffset + 1);
	}

	std::wstring Utility::GetFileExtension(const std::wstring& filePath)
	{
		std::wstring fileName = RemoveBasePath(filePath);
		size_t extOffset = fileName.rfind(L'.');
		if (extOffset == std::wstring::npos)
			return L"";

		return fileName.substr(extOffset + 1);
	}

	std::string Utility::RemoveExtension(const std::string& filePath)
	{
		return filePath.substr(0, filePath.rfind('.'));
	}

	std::wstring Utility::RemoveExtension(const std::wstring& filePath)
	{
		return filePath.substr(0, filePath.rfind(L'.'));
	}
}