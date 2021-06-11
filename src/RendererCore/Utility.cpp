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

#include "Utility.h"
#include <locale>

std::wstring Utility::AnsiToWideString(const std::string& str)
{
	wchar_t wstr[MAX_PATH];
	if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str.c_str(), -1, wstr, MAX_PATH))
		wstr[0] = L'\0';
	return wstr;
}

std::string Utility::WideStringToAnsi(const std::wstring& wstr)
{
	char str[MAX_PATH];
	if (!WideCharToMultiByte(CP_ACP, MB_PRECOMPOSED, wstr.c_str(), -1, str, MAX_PATH, nullptr, nullptr))
		str[0] = '\0';
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
		return filePath.substr(lastSlash + 1, std::string::npos);
	else if ((lastSlash = filePath.rfind('\\')) != std::string::npos)
		return filePath.substr(lastSlash + 1, std::string::npos);
	else
		return filePath;
}

std::wstring Utility::RemoveBasePath(const std::wstring& filePath)
{
	size_t lastSlash;
	if ((lastSlash = filePath.rfind(L'/')) != std::wstring::npos)
		return filePath.substr(lastSlash + 1, std::wstring::npos);
	else if ((lastSlash = filePath.rfind(L'\\')) != std::wstring::npos)
		return filePath.substr(lastSlash + 1, std::wstring::npos);
	else
		return filePath;
}